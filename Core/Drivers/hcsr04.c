#include "hcsr04.h"
 
static TIM_HandleTypeDef  *_htim;
static GPIO_TypeDef       *_trig_port;
static uint16_t            _trig_pin;
static GPIO_TypeDef       *_echo_port;
static uint16_t            _echo_pin;
 
/* TIM must be configured at 1 MHz (1 tick = 1 µs) */
void hcsr04_init(TIM_HandleTypeDef *htim,
                 GPIO_TypeDef *trig_port, uint16_t trig_pin,
                 GPIO_TypeDef *echo_port, uint16_t echo_pin)
{
    _htim      = htim;
    _trig_port = trig_port;
    _trig_pin  = trig_pin;
    _echo_port = echo_port;
    _echo_pin  = echo_pin;
 
    HAL_TIM_Base_Start(_htim);
}
 
static void delay_us(uint32_t us)
{
    __HAL_TIM_SET_COUNTER(_htim, 0);
    while (__HAL_TIM_GET_COUNTER(_htim) < us);
}
 
uint32_t hcsr04_get_distance_cm(void)
{
    uint32_t t_start, t_end, elapsed;
 
    /* Send 10 µs trigger pulse */
    HAL_GPIO_WritePin(_trig_port, _trig_pin, GPIO_PIN_SET);
    delay_us(10);
    HAL_GPIO_WritePin(_trig_port, _trig_pin, GPIO_PIN_RESET);
 
    /* Wait for echo rising edge (timeout 30 ms) */
    __HAL_TIM_SET_COUNTER(_htim, 0);
    while (HAL_GPIO_ReadPin(_echo_port, _echo_pin) == GPIO_PIN_RESET)
    {
        if (__HAL_TIM_GET_COUNTER(_htim) > 30000) return 0;
    }
 
    /* Measure echo pulse width */
    __HAL_TIM_SET_COUNTER(_htim, 0);
    t_start = 0;
    while (HAL_GPIO_ReadPin(_echo_port, _echo_pin) == GPIO_PIN_SET)
    {
        if (__HAL_TIM_GET_COUNTER(_htim) > 30000) return 0;
    }
    t_end = __HAL_TIM_GET_COUNTER(_htim);
 
    elapsed = t_end - t_start;
 
    /* distance (cm) = pulse_width (µs) / 58 */
    return elapsed / 58;
}
