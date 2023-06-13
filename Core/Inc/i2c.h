#ifndef __I2C_H
#define __I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
  
void MX_I2C1_Init(void);

extern I2C_HandleTypeDef hi2c1;



#ifdef __cplusplus
}
#endif

#endif 
