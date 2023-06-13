#ifndef __USART_H
#define __USART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
  
void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;



#ifdef __cplusplus
}
#endif

#endif 
