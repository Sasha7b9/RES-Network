// 2022/04/20 08:54:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#ifndef __USB_DEVICE__H__
#define __USB_DEVICE__H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "usbd_def.h"

void MX_USB_DEVICE_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* __USB_DEVICE__H__ */
