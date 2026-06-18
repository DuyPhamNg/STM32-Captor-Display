#ifndef HCSR04_H
#define HCSR04_H
 
#include "stm32l4xx_hal.h"
#include <stdint.h>
 
void     hcsr04_init(TIM_HandleTypeDef *htim, GPIO_TypeDef *trig_port, uint16_t trig_pin,
                                              GPIO_TypeDef *echo_port, uint16_t echo_pin);
uint32_t hcsr04_get_distance_cm(void);
 
#endif
 
