#ifndef __GPIO_H
#define __GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

void MX_GPIO_Init(void);

#define BUTTON1_Pin GPIO_PIN_0
#define BUTTON1_GPIO_Port GPIOA
#define BUTTON2_Pin GPIO_PIN_1
#define BUTTON2_GPIO_Port GPIOA
#define LOAD_IC_Pin GPIO_PIN_1
#define LOAD_IC_GPIO_Port GPIOB
#define DATA_IC_Pin GPIO_PIN_10
#define DATA_IC_GPIO_Port GPIOB
#define CLOCK_IC_Pin GPIO_PIN_11
#define CLOCK_IC_GPIO_Port GPIOB
#define DIR_Pin GPIO_PIN_15
#define DIR_GPIO_Port GPIOB
#define BUTTON3_Pin GPIO_PIN_7
#define BUTTON3_GPIO_Port GPIOB

#ifdef __cplusplus
}
#endif

#endif 
