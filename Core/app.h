#ifndef APP_H
#define APP_H
 
#include "stm32l4xx_hal.h"
 
void app_init(I2C_HandleTypeDef *hi2c, TIM_HandleTypeDef *htim, ADC_HandleTypeDef *hadc);
void app_run(void);
 
#endif
 
