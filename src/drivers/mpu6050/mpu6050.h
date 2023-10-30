#ifndef __DRIVERS_MPU6050_H__
#define __DRIVERS_MPU6050_H__

#include <stdint.h>

#define __DRIVERS_MPU6050_H_MPU6050_PIN_CL__    (12)
#define __DRIVERS_MPU6050_H_MPU6050_PIN_DA__    (13)

int8_t Mpu6050_Init(void);
int8_t Mpu6050_Start(void);
int8_t Mpu6050_SendAddress(char address);
int8_t Mpu6050_SendData(char data);
char Mpu6050_ReadData(void);
int8_t Mpu6050_Stop(void);
int8_t Mpu6050_Read(char address, char *data);
int8_t Mpu6050_BurstRead(char address, char *data, uint8_t reads);
int8_t Mpu6050_Write(char address, char data);
int8_t Mpu6050_BurstWrite(char address, char *data);
// int8_t Mpu6050_GetSeconds(char *seconds);
// int8_t Mpu6050_GetMinutes(char *minutes);
// int8_t Mpu6050_GetHours(char *hours);
// int8_t Mpu6050_GetTime(int8_t *hours, int8_t *minutes, int8_t *seconds);
// int8_t Mpu6050_SetSeconds(int8_t seconds);
// int8_t Mpu6050_SetMinutes(int8_t minutes);
// int8_t Mpu6050_SetHours(int8_t hours);

#endif