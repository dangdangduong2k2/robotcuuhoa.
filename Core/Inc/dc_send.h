#ifndef __DC_SEND_H
#define __DC_SEND_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "usart.h"
#include "stdio.h"
#include <string.h>
void DC_Servo_Driver_UART_MSD_Restart_Driver(void);

void DC_Servo_Driver_UART_MSD_Moving_Set(uint32_t acceleration,
                                         uint32_t velocity,
                                         int32_t position);
void DC_Servo_Driver_UART_MSD_Moving_Stop(void);
void DC_Servo_Driver_UART_MSD_Moving_Start(void);

#ifdef __cplusplus
}
#endif

#endif 
