#include <stdint.h>
#include "src/headers/stm32f412zx.h"
#include "src/drivers/mpu6050/mpu6050.h"

int8_t Mpu6050_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    GPIOD->MODER |= (0b10 << (__DRIVERS_MPU6050_H_MPU6050_PIN_CL__ * 2)) |
                    (0b10 << (__DRIVERS_MPU6050_H_MPU6050_PIN_DA__ * 2));

    GPIOD->AFR[1] |= (0b0100 << ((__DRIVERS_MPU6050_H_MPU6050_PIN_CL__ - 8) * 4)) |
                    (0b0100 << ((__DRIVERS_MPU6050_H_MPU6050_PIN_DA__ - 8) * 4));

    GPIOD->OTYPER |= (0b1 << __DRIVERS_MPU6050_H_MPU6050_PIN_CL__) |
                    (0b1 << __DRIVERS_MPU6050_H_MPU6050_PIN_DA__);

    GPIOD->OSPEEDR |= (0b11 << (__DRIVERS_MPU6050_H_MPU6050_PIN_CL__ * 2)) |
                        (0b11 << (__DRIVERS_MPU6050_H_MPU6050_PIN_DA__ * 2));

    GPIOD->PUPDR |= (0b01 << (__DRIVERS_MPU6050_H_MPU6050_PIN_CL__ * 2)) |
                        (0b01 << (__DRIVERS_MPU6050_H_MPU6050_PIN_DA__ * 2));

    I2C1->CR1 = I2C_CR1_SWRST;

    I2C1->CR1 &= ~I2C_CR1_SWRST;

    I2C1->CR2 |= I2C_CR2_ITERREN;
    I2C1->CR2 |= 0x10;

    I2C1->CCR = 0x50;

    I2C1->TRISE = 0x11;

    I2C1->CR1 |= I2C_CR1_PE;

    return 0;
}

int8_t Mpu6050_Start(void)
{

    I2C1->CR1 |= I2C_CR1_START;

    while (!(I2C1->SR1 & I2C_SR1_SB));

    return 0;
}

int8_t Mpu6050_SendAddress(char address)
{
    uint16_t reg;

    I2C1->DR = address;//(address << 1) | 0x01;

    while (!(I2C1->SR1 & I2C_SR1_ADDR));

    reg = I2C1->SR1 | I2C1->SR2;
    
    return 0;
}

int8_t Mpu6050_SendData(char data)
{
    while (!(I2C1->SR1 & I2C_SR1_TXE));

    I2C1->DR = data;

    while (!(I2C1->SR1 & I2C_SR1_BTF));

    return 0;
}

char Mpu6050_ReadData(void)
{

    while (!(I2C1->SR1 & I2C_SR1_RXNE));

    return I2C1->DR;

}

int8_t Mpu6050_Stop(void)
{
    uint16_t reg;

    I2C1->CR1 &= ~I2C_CR1_ACK;

    reg = I2C1->SR1 | I2C1->SR2;

    I2C1->CR1 |= I2C_CR1_STOP;

    return 0;
}

int8_t Mpu6050_Read(char address, char *data)
{
    uint16_t reg;

    I2C1->CR1 |= I2C_CR1_START |
                I2C_CR1_ACK;

    while (!(I2C1->SR1 & I2C_SR1_SB));

    I2C1->DR = 0b11010000;

    while (!(I2C1->SR1 & I2C_SR1_ADDR));

    reg = I2C1->SR1 | I2C1->SR2;

    while (!(I2C1->SR1 & I2C_SR1_TXE));

    I2C1->DR = address;

    while (!(I2C1->SR1 & I2C_SR1_BTF));

    I2C1->CR1 |= I2C_CR1_START;

    while (!(I2C1->SR1 & I2C_SR1_SB));

    I2C1->DR = 0b11010001;

    while (!(I2C1->SR1 & I2C_SR1_ADDR));

    reg = I2C1->SR1 | I2C1->SR2;

    I2C1->CR1 &= ~I2C_CR1_ACK;

    reg = I2C1->SR1 | I2C1->SR2;

    I2C1->CR1 |= I2C_CR1_STOP;

    while (!(I2C1->SR1 & I2C_SR1_RXNE));

    *data = I2C1->DR;

    return 0;
}

int8_t Mpu6050_BurstRead(char address, char *data, uint8_t reads)
{
    uint16_t reg;

    I2C1->CR1 |= I2C_CR1_START |
                I2C_CR1_ACK;

    while (!(I2C1->SR1 & I2C_SR1_SB));

    I2C1->DR = 0b11010000;

    while (!(I2C1->SR1 & I2C_SR1_ADDR));

    reg = I2C1->SR1 | I2C1->SR2;

    while (!(I2C1->SR1 & I2C_SR1_TXE));

    I2C1->DR = address;

    while (!(I2C1->SR1 & I2C_SR1_BTF));

    I2C1->CR1 |= I2C_CR1_START;

    while (!(I2C1->SR1 & I2C_SR1_SB));

    I2C1->DR = 0b11010001;

    while (!(I2C1->SR1 & I2C_SR1_ADDR));

    reg = I2C1->SR1 | I2C1->SR2;

    for (uint8_t i = 0; i < reads; i++, data++)
    {
        if (i + 1 == reads)
        {
            I2C1->CR1 &= ~I2C_CR1_ACK;

            I2C1->CR1 |= I2C_CR1_STOP;
        }

        while (!(I2C1->SR1 & I2C_SR1_RXNE));

        *data = I2C1->DR;
    }

    return 0;
}

int8_t Mpu6050_Write(char address, char data)
{
    uint16_t reg;

    I2C1->CR1 |= I2C_CR1_START |
                I2C_CR1_ACK;

    while (!(I2C1->SR1 & I2C_SR1_SB));

    I2C1->DR = 0b11010000;

    while (!(I2C1->SR1 & I2C_SR1_ADDR));

    reg = I2C1->SR1 | I2C1->SR2;

    while (!(I2C1->SR1 & I2C_SR1_TXE));

    I2C1->DR = address;

    while (!(I2C1->SR1 & I2C_SR1_BTF));

    while (!(I2C1->SR1 & I2C_SR1_TXE));

    I2C1->DR = data;

    while (!(I2C1->SR1 & I2C_SR1_BTF));

    I2C1->CR1 |= I2C_CR1_STOP;

    return 0;
}

int8_t Mpu6050_BurstWrite(char address, char *data)
{
    uint16_t reg;

    I2C1->CR1 |= I2C_CR1_START |
                I2C_CR1_ACK;

    while (!(I2C1->SR1 & I2C_SR1_SB));

    I2C1->DR = 0b11010000;

    while (!(I2C1->SR1 & I2C_SR1_ADDR));

    reg = I2C1->SR1 | I2C1->SR2;

    while (!(I2C1->SR1 & I2C_SR1_TXE));

    I2C1->DR = address;

    while (!(I2C1->SR1 & I2C_SR1_BTF));

    while (*data != '\0')
    {
        while (!(I2C1->SR1 & I2C_SR1_TXE));

        I2C1->DR = *(data++);

        while (!(I2C1->SR1 & I2C_SR1_BTF));
    }

    I2C1->CR1 |= I2C_CR1_STOP;

    return 0;
}

// int8_t Mpu6050_GetSeconds(char *seconds)
// {
//     char dataSeconds;
//     Mpu6050_Read(0x00, &dataSeconds);

//     char unitSeconds = dataSeconds & 0b1111;
//     char tensSeconds = dataSeconds >> 4;

//     *seconds = (tensSeconds * 10) + unitSeconds;

//     return 0;
// }

// int8_t Mpu6050_GetMinutes(char *minutes)
// {
//     char dataMinutes;
//     Mpu6050_Read(0x01, &dataMinutes);

//     char unitMinutes = dataMinutes & 0b1111;
//     char tensMinutes = dataMinutes >> 4;

//     *minutes = (tensMinutes * 10) + unitMinutes;

//     return 0;
// }

// int8_t Mpu6050_GetHours(char *hours)
// {
//     char dataHours;
//     Mpu6050_Read(0x02, &dataHours);

//     char unitHours = dataHours & 0b1111;
//     char tensHours = (dataHours >> 4) & 0b11;

//     *hours = (tensHours * 10) + unitHours;

//     return 0;
// }

// int8_t Mpu6050_GetTime(int8_t *hours, int8_t *minutes, int8_t *seconds)
// {

//     char data[3];

//     Mpu6050_BurstRead(0x00, data, 3);

//     *seconds = ((data[0] >> 4) * 10) + (data[0] & 0b1111);
//     *minutes = ((data[1] >> 4) * 10) + (data[1] & 0b1111);
//     *hours = (((data[2] >> 4) & 0b11) * 10) + (data[2] & 0b1111);

//     return 0;
// }

// int8_t Mpu6050_SetSeconds(int8_t seconds)
// {
//     char unitSeconds = seconds % 10;
//     char tensSeconds = seconds / 10;

//     char dataSeconds = (tensSeconds << 4) | unitSeconds;

//     Mpu6050_Write(0x00, dataSeconds);

//     return 0;
// }

// int8_t Mpu6050_SetMinutes(int8_t minutes)
// {
//     char unitMinutes = minutes % 10;
//     char tensMinutes = minutes / 10;

//     char dataMinutes = (tensMinutes << 4) | unitMinutes;

//     Mpu6050_Write(0x01, dataMinutes);

//     return 0;
// }

// int8_t Mpu6050_SetHours(int8_t hours)
// {
//     char unitHours = hours % 10;
//     char tensHours = hours / 10;

//     char dataHours = (tensHours << 4) | unitHours;

//     Mpu6050_Write(0x02, dataHours);

//     return 0;
// }