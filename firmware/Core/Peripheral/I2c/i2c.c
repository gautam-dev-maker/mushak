/**
 ******************************************************************************
 * @file           : i2c.h
 * @brief          : source code for I2C library
 * @author         : Gautam Agrawal
 *
 * Created on: July 19, 2022
 ******************************************************************************
 * @attention
 *
 * Copyright (c)  2022 Society of Robotics and Automation
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 **/

#include "i2c.h"

HAL_StatusTypeDef i2c_init(i2c_dev_t *dev)
{
    HAL_StatusTypeDef ret1 = HAL_I2C_Init(dev->i2c_handle);
    HAL_Delay(20);
    HAL_StatusTypeDef ret2 = HAL_I2C_IsDeviceReady(dev->i2c_handle, dev->dev_addr, 2, I2C_TIMEOUT);
    return ret1 & ret2;
}

HAL_StatusTypeDef
i2c_read_8_bit(i2c_dev_t *dev, uint16_t reg_addr, uint8_t *out)
{
    uint8_t data[2] = {reg_addr >> 8, reg_addr};
    HAL_StatusTypeDef ret1 = HAL_I2C_Master_Transmit(dev->i2c_handle, dev->dev_addr, data, 2, I2C_TIMEOUT);
    HAL_StatusTypeDef ret2 = HAL_I2C_Master_Receive(dev->i2c_handle, dev->dev_addr, out, 1, I2C_TIMEOUT);
    return ret1 & ret2;
}

HAL_StatusTypeDef
i2c_read_16_bit(i2c_dev_t *dev, uint16_t reg_addr, uint16_t *out)
{
    uint8_t data[2] = {reg_addr >> 8, reg_addr};
    uint8_t out_data[2] = {0,0};
    HAL_StatusTypeDef ret1 = HAL_I2C_Master_Transmit(dev->i2c_handle, dev->dev_addr, data, 2, I2C_TIMEOUT);
    HAL_StatusTypeDef ret2 = HAL_I2C_Master_Receive(dev->i2c_handle, dev->dev_addr, out_data, 2, I2C_TIMEOUT);
    *out = out_data[0];
    *out <<= 8;
    *out = out_data[1];
    return ret1 & ret2;
}

HAL_StatusTypeDef i2c_write_8_bit(i2c_dev_t *dev, uint16_t reg_addr, uint8_t in)
{
    uint8_t data[3] = {reg_addr >> 8, reg_addr, in};
    return HAL_I2C_Master_Transmit(dev->i2c_handle, dev->dev_addr, data, 3, I2C_TIMEOUT);
}

HAL_StatusTypeDef i2c_write_16_bit(i2c_dev_t *dev, uint16_t reg_addr, uint16_t in)
{
    uint8_t data[4] = {reg_addr >> 8, reg_addr, in >> 8, in};
    return HAL_I2C_Master_Transmit(dev->i2c_handle, dev->dev_addr, data, 4, I2C_TIMEOUT);
}