#ifndef __MAIN_H
#define __MAIN_H
 
#include "stm32l4xx_hal.h"
 
/* ── Exported handles ───────────────────────────────────────────────────── */
extern ADC_HandleTypeDef  hadc1;
extern I2C_HandleTypeDef  hi2c1;
extern TIM_HandleTypeDef  htim2;
extern UART_HandleTypeDef huart2;
 
/* ── Pin definitions ────────────────────────────────────────────────────── */
 
/* User button B1 */
#define B1_Pin        GPIO_PIN_13
#define B1_GPIO_Port  GPIOC
 
/* Green LED LD2 */
#define LD2_Pin       GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
 
/* HC-SR04 */
#define TRIG_Pin       GPIO_PIN_0
#define TRIG_GPIO_Port GPIOA
#define ECHO_Pin       GPIO_PIN_1
#define ECHO_GPIO_Port GPIOA
 
/* ── Error handler ──────────────────────────────────────────────────────── */
void Error_Handler(void);
 
#endif
