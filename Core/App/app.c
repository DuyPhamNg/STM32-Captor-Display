#include "app.h"
#include "../Drivers/lcd_i2c.h"
#include "../Drivers/hcsr04.h"
#include "../Drivers/mpu6050.h"
 
/* ── State machine ─────────────────────────────────────────────────────── */
typedef enum {
    STATE_WELCOME   = 0,  /* "Bonjour comment ca va" */
    STATE_DISTANCE  = 1,  /* distance HC-SR04        */
    STATE_TEMP      = 2,  /* temperature interne ADC */
    STATE_ANGLE     = 3,  /* roll / pitch MPU6050    */
    STATE_COUNT     = 4
} AppState;
 
/* ── Private variables ─────────────────────────────────────────────────── */
static AppState         current_state = STATE_WELCOME;
static ADC_HandleTypeDef *_hadc;
static uint8_t          btn_prev = GPIO_PIN_SET;  /* B1 active low */
 
/* ── Button handling ───────────────────────────────────────────────────── */
static uint8_t button_pressed(void)
{
    uint8_t btn_curr = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);  /* B1 = PC13 */
 
    if (btn_prev == GPIO_PIN_SET && btn_curr == GPIO_PIN_RESET)
    {
        HAL_Delay(20);  /* debounce */
        btn_prev = btn_curr;
        return 1;
    }
    btn_prev = btn_curr;
    return 0;
}
 
/* ── Temperature from internal ADC ────────────────────────────────────── */
static float read_temperature_adc(void)
{
    uint32_t sum = 0;
    for (int i = 0; i < 8; i++)
    {
        HAL_ADC_Start(_hadc);
        HAL_ADC_PollForConversion(_hadc, 10);
        sum += HAL_ADC_GetValue(_hadc);
        HAL_ADC_Stop(_hadc);
    }
    uint32_t avg = sum / 8;
 
    /* Convert ADC value to voltage then to temperature
     * Vref = 3.3V, 12-bit ADC → 4096 steps
     * NTC or internal sensor linear approximation */
    float voltage = (float)avg * 3.3f / 4096.0f;
    float temperature = (voltage - 0.5f) * 100.0f;  /* adjust for your sensor */
    return temperature;
}
 
/* ── State display functions ───────────────────────────────────────────── */
static void display_welcome(void)
{
    lcd_printf(0, 0, "  Bonjour !");
    lcd_printf(1, 0, "Comment ca va ?");
}
 
static void display_distance(void)
{
    uint32_t dist = hcsr04_get_distance_cm();
    if (dist == 0 || dist > 400)
    {
        lcd_printf(0, 0, "Distance:");
        lcd_printf(1, 0, "Hors portee");
    }
    else
    {
        lcd_printf(0, 0, "Distance:");
        lcd_printf(1, 0, "%lu cm", dist);
    }
}
 
static void display_temperature(void)
{
    float temp = read_temperature_adc();
    lcd_printf(0, 0, "Temperature:");
    lcd_printf(1, 0, "%.1f C", temp);
}
 
static void display_angle(void)
{
    MPU6050_Data imu;
    if (mpu6050_read(&imu) != MPU6050_OK)
    {
        lcd_printf(0, 0, "IMU erreur");
        lcd_printf(1, 0, "Verif capteur");
        return;
    }
    lcd_printf(0, 0, "Roll : %.1f", imu.roll);
    lcd_printf(1, 0, "Pitch: %.1f", imu.pitch);
}
 
/* ── Public API ────────────────────────────────────────────────────────── */
void app_init(I2C_HandleTypeDef *hi2c, TIM_HandleTypeDef *htim, ADC_HandleTypeDef *hadc)
{
    _hadc = hadc;
 
    lcd_init(hi2c);
    hcsr04_init(htim, GPIOA, GPIO_PIN_0, GPIOA, GPIO_PIN_1);  /* adjust pins */
 
    if (mpu6050_init(hi2c) != MPU6050_OK)
    {
        lcd_printf(0, 0, "MPU6050");
        lcd_printf(1, 0, "Non detecte!");
        HAL_Delay(2000);
    }
 
    lcd_clear();
    display_welcome();
}
 
void app_run(void)
{
    /* Check button → advance state */
    if (button_pressed())
    {
        current_state = (AppState)((current_state + 1) % STATE_COUNT);
        lcd_clear();
    }
 
    /* Refresh display for current state */
    switch (current_state)
    {
        case STATE_WELCOME:   display_welcome();     break;
        case STATE_DISTANCE:  display_distance();    break;
        case STATE_TEMP:      display_temperature(); break;
        case STATE_ANGLE:     display_angle();       break;
        default:              break;
    }
 
    HAL_Delay(200);  /* refresh rate 5 Hz */
}
