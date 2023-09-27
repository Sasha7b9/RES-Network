// 2023/06/15 14:13:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f3xx_hal.h>


Pin         pinWP(GPIOB, GPIO_PIN_0, GPIO_MODE_OUTPUT_PP, GPIO_PULLUP);
PinOutputPP pinBEEP(GPIOB, GPIO_PIN_4, GPIO_PULLUP);
PinAnalog   pinADC(GPIOA, GPIO_PIN_3);
Pin         pinSCL(GPIOB, GPIO_PIN_6, GPIO_MODE_AF_OD, GPIO_AF4_I2C1);
Pin         pinSDA(GPIOB, GPIO_PIN_7, GPIO_MODE_AF_OD, GPIO_AF4_I2C1);
PinAF_PP    pinSCK_SPI1(GPIOA, GPIO_PIN_5, GPIO_PULLUP);
PinAF_PP    pinMOSI_SPI1(GPIOA, GPIO_PIN_7, GPIO_PULLUP);
PinInput    pinMISO_SPI1(GPIOA, GPIO_PIN_6, GPIO_NOPULL);
PinOutputPP pinNSS_SPI1(GPIOA, GPIO_PIN_4, GPIO_NOPULL);
PinAF_PP    pinTX(GPIOA, GPIO_PIN_9, GPIO_NOPULL);
PinInput    pinRX(GPIOA, GPIO_PIN_10, GPIO_NOPULL);
PinInput    pinKey1(GPIOB, GPIO_PIN_8, GPIO_PULLUP);
PinInput    pinKey2(GPIOB, GPIO_PIN_9, GPIO_PULLUP);
PinOutputPP pinCS_HC12(GPIOA, GPIO_PIN_6, GPIO_NOPULL);

PinOutputPP pinRESET_ST(GPIOB, GPIO_PIN_11, GPIO_NOPULL);
PinOutputPP pinCS_ST(GPIOB, GPIO_PIN_12, GPIO_PULLDOWN);
Pin         pinSCL_SPI2(GPIOB, GPIO_PIN_13, GPIO_MODE_AF_PP, GPIO_PULLDOWN, GPIO_AF5_SPI2);
PinOutputPP pinDC_ST(GPIOB, GPIO_PIN_14, GPIO_NOPULL);
Pin         pinMOSI_SPI2(GPIOB, GPIO_PIN_15, GPIO_MODE_AF_PP, GPIO_PULLDOWN, GPIO_AF5_SPI2);


void Pin::Init()
{
    GPIO_InitTypeDef is =
    {
        pin,
        mode,
        pull,
        GPIO_SPEED_FREQ_HIGH,
        alternate
    };

    HAL_GPIO_Init(gpio, &is);
}


void Pin::ToLow()
{
    HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET);
}


void Pin::ToHi()
{
    HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET);
}


void Pin::Set(bool hi)
{
    HAL_GPIO_WritePin(gpio, pin, hi ? GPIO_PIN_SET : GPIO_PIN_RESET);
}


bool Pin::IsHi() const
{
    return HAL_GPIO_ReadPin(gpio, pin) != GPIO_PIN_RESET;
}


bool Pin::IsLow() const
{
    return HAL_GPIO_ReadPin(gpio, pin) == GPIO_PIN_RESET;
}
