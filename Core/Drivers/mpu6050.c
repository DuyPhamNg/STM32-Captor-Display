#include "mpu6050.h"
#include <math.h>
 
static I2C_HandleTypeDef *_hi2c;
 
static MPU6050_Status write_reg(uint8_t reg, uint8_t value)
{
    uint8_t buf[2] = {reg, value};
    if (HAL_I2C_Master_Transmit(_hi2c, MPU6050_ADDR, buf, 2, 10) != HAL_OK)
        return MPU6050_ERROR;
    return MPU6050_OK;
}
 
static MPU6050_Status read_regs(uint8_t reg, uint8_t *buf, uint8_t len)
{
    if (HAL_I2C_Master_Transmit(_hi2c, MPU6050_ADDR, &reg, 1, 10) != HAL_OK)
        return MPU6050_ERROR;
    if (HAL_I2C_Master_Receive(_hi2c, MPU6050_ADDR, buf, len, 10) != HAL_OK)
        return MPU6050_ERROR;
    return MPU6050_OK;
}
 
MPU6050_Status mpu6050_init(I2C_HandleTypeDef *hi2c)
{
    _hi2c = hi2c;
    uint8_t who_am_i;
 
    /* Check device presence */
    if (read_regs(MPU6050_REG_WHO_AM_I, &who_am_i, 1) != MPU6050_OK)
        return MPU6050_ERROR;
    if (who_am_i != 0x68)
        return MPU6050_ERROR;
 
    /* Wake up device (clear sleep bit) */
    if (write_reg(MPU6050_REG_PWR_MGMT_1, 0x00) != MPU6050_OK)
        return MPU6050_ERROR;
 
    HAL_Delay(100);
    return MPU6050_OK;
}
 
MPU6050_Status mpu6050_read(MPU6050_Data *data)
{
    uint8_t buf[14];
 
    /* Read 14 bytes: ACCEL (6) + TEMP (2) + GYRO (6) */
    if (read_regs(MPU6050_REG_ACCEL_XOUT, buf, 14) != MPU6050_OK)
        return MPU6050_ERROR;
 
    int16_t ax_raw = (int16_t)(buf[0]  << 8 | buf[1]);
    int16_t ay_raw = (int16_t)(buf[2]  << 8 | buf[3]);
    int16_t az_raw = (int16_t)(buf[4]  << 8 | buf[5]);
    int16_t gx_raw = (int16_t)(buf[8]  << 8 | buf[9]);
    int16_t gy_raw = (int16_t)(buf[10] << 8 | buf[11]);
    int16_t gz_raw = (int16_t)(buf[12] << 8 | buf[13]);
 
    /* Convert to physical units
     * Default range: ±2g → 16384 LSB/g, ±250°/s → 131 LSB/°/s */
    data->accel_x = (float)ax_raw / 16384.0f * 9.81f;
    data->accel_y = (float)ay_raw / 16384.0f * 9.81f;
    data->accel_z = (float)az_raw / 16384.0f * 9.81f;
    data->gyro_x  = (float)gx_raw / 131.0f;
    data->gyro_y  = (float)gy_raw / 131.0f;
    data->gyro_z  = (float)gz_raw / 131.0f;
 
    /* Roll and pitch from accelerometer */
    data->roll  = atan2f(data->accel_y, data->accel_z) * 180.0f / (float)M_PI;
    data->pitch = atan2f(-data->accel_x,
                  sqrtf(data->accel_y * data->accel_y + data->accel_z * data->accel_z))
                  * 180.0f / (float)M_PI;
 
    return MPU6050_OK;
}
