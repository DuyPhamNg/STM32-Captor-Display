#ifndef MPU6050_H
#define MPU6050_H
 
#include "stm32l4xx_hal.h"
#include <stdint.h>
 
#define MPU6050_ADDR        (0x68 << 1)
#define MPU6050_REG_WHO_AM_I    0x75
#define MPU6050_REG_PWR_MGMT_1  0x6B
#define MPU6050_REG_ACCEL_XOUT  0x3B
#define MPU6050_REG_GYRO_XOUT   0x43
 
typedef struct {
    float accel_x, accel_y, accel_z;  /* m/s² */
    float gyro_x,  gyro_y,  gyro_z;   /* °/s  */
    float roll,    pitch;              /* degrees */
} MPU6050_Data;
 
typedef enum {
    MPU6050_OK    = 0,
    MPU6050_ERROR = 1
} MPU6050_Status;
 
MPU6050_Status mpu6050_init(I2C_HandleTypeDef *hi2c);
MPU6050_Status mpu6050_read(MPU6050_Data *data);
 
#endif
