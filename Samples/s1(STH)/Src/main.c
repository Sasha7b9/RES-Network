
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_conf.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"
#include "stdio.h"
#include "ds18b20.h"
#include "stdbool.h"

#define SENS_ID		0x01

#define MCU_VOLTAGE	3.3
#define RES_UP		2.0
#define RES_DOWN	3.0

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
I2C_HandleTypeDef hi2c1;
SPI_HandleTypeDef hspi1;
TIM_HandleTypeDef htim2;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* Private variables ---------------------------------------------------------*/

#define AVERAGE_SIZE	3
typedef struct{
	float val;
	uint16_t buf[AVERAGE_SIZE];
	uint8_t cnt;
	bool fFull;
}Average_t;

char buffer_USB[60] = {0};
uint8_t radio_buf[6] = {0};
uint8_t dt[8];
uint16_t raw_temper;
float temper;
float hum;
Average_t hum_avrg = {0};

GPIO_PinState timFlag = GPIO_PIN_RESET;
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);

//==========================================================
void IncertVar(uint16_t value, Average_t *avrg)
{
	if(avrg->cnt >= AVERAGE_SIZE)
	{
		avrg->cnt = 0;
		if(!avrg->fFull) avrg->fFull = true;
	}
	avrg->buf[avrg->cnt] = value;
	avrg->cnt++;
}

void GenerateAverageVar(Average_t *avrg, float fact)
{
	uint32_t val = 0;
	uint8_t avSize = 1;
	if(avrg->fFull) avSize = AVERAGE_SIZE;
	else {
		if(avrg->cnt != 0) avSize = avrg->cnt;
		else return;
	}

	for(int i = 0; i < avSize; i++)
		val += avrg->buf[i];
	avrg->val = (val / avSize) * fact;
}
//==========================================================

void pack2BytesSFloat(uint8_t index, double bytes)
{
	float  tmp_fraction;
	double tmp_integer;

	tmp_fraction = modf(bytes, &tmp_integer);

	radio_buf[index] = (int8_t)tmp_integer;
	radio_buf[index+1] = (int8_t)(100 * tmp_fraction);
}

uint8_t calcTelemetryCrc(void)
{
    uint8_t crc = 0xFF;

    for(uint8_t i = 0; i < 5; i++)
    {
        crc ^= radio_buf[i];

        for(uint8_t j = 0; j < 4; j++)
            crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
    }
    return crc;
}

float ADC_Convert(uint16_t value)
{
	float humidity = 0;
	float voltage = 0;
	if(value > 4095) value = 4095;

	voltage = MCU_VOLTAGE*value/4095.0;	//adc to voltage
	voltage = voltage*(RES_UP+RES_DOWN)/RES_DOWN;//voltage with resistance
	humidity = (voltage - 0.826)/0.0315;

	return humidity;
}

float AdcToHum(uint16_t value)
{
	float humidity = 0;
	float voltage = 0;
	if(value > 4095) value = 4095;

	voltage = MCU_VOLTAGE*value/4095.0;	//adc to voltage
	voltage = voltage*(RES_UP+RES_DOWN)/RES_DOWN;//voltage with resistance
	humidity = (voltage - 0.826)/0.0315;

	return humidity;
}

/* Private function prototypes -----------------------------------------------*/


int main(void)
{
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  //MX_USART2_UART_Init();
  MX_USB_DEVICE_Init();
  MX_TIM2_Init();

  port_init();
  ds18b20_init(SKIP_ROM);

  //timFlag = GPIO_PIN_SET;
  while (1)
  {
		ds18b20_MeasureTemperCmd(SKIP_ROM, 0);
		HAL_Delay(800);
		ds18b20_ReadStratcpad(SKIP_ROM, dt, 0);
		raw_temper = ((uint16_t)dt[1]<<8)|dt[0];
		//temper = ds18b20_Convert(raw_temper);
		//if(temper>125) temper = temper - 128.0;

//		if(ds18b20_GetSign(raw_temper))sign = -1.0;
//		else sign = 1.0;
		temper = ds18b20_Convert(raw_temper);
		//temper = sign*temper;



		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1, 100);
		IncertVar(HAL_ADC_GetValue(&hadc1), &hum_avrg); //hum = ADC_Convert( HAL_ADC_GetValue(&hadc1));
		GenerateAverageVar(&hum_avrg, 1);
		hum = AdcToHum(hum_avrg.val);
		HAL_ADC_Stop(&hadc1);

		radio_buf[0] = SENS_ID;
		pack2BytesSFloat(1, temper);
		pack2BytesSFloat(3, hum);
		radio_buf[5] = calcTelemetryCrc();

		memset(buffer_USB,0,60);
		sprintf(buffer_USB, "STH:%.2f:%.2f\n", temper, hum);
		CDC_Transmit_FS((uint8_t*)buffer_USB, strlen(buffer_USB));
		HAL_UART_Transmit(&huart1, radio_buf, 6, 1000);
		HAL_Delay(100);
  }
  /* USER CODE END 3 */
}
//=================================================================================
//
//=================================================================================
/*

void TIM2_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim2);

  if(timFlag != GPIO_PIN_RESET)
  {


  }

}
*/
//=================================================================================
//
//=================================================================================
/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_USB;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Common config 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  HAL_ADC_Init(&hadc1);

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_71CYCLES_5;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

}

/* I2C1 init function */
void MX_I2C1_Init(void)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  HAL_I2C_Init(&hi2c1);
}

/* SPI1 init function */
void MX_SPI1_Init(void)
{
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  HAL_SPI_Init(&hspi1);

}

/* TIM2 init function */
void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7199;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 10000;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim2);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

}

void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);
}

/* USART2 init function */
void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart2);
}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB11*/
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//  GPIO_InitStruct.Pin = GPIO_PIN_2;
//  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
