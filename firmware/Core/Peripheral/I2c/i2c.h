/**
 ******************************************************************************
 * @file           : i2c.h
 * @brief          : header file for I2C library
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

#ifndef _I2C_H
#define _I2C_H

#include "stdio.h"
#include "stdlib.h"
#include "stm32f4xx_hal.h"

#define I2C_TIMEOUT 50

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct i2c_dev_t_
    {
        I2C_HandleTypeDef i2c_handle;
        uint8_t dev_addr;

    } i2c_dev_t;

    HAL_StatusTypeDef i2c_init(i2c_dev_t *dev);

    HAL_StatusTypeDef i2c_read_8_bit(i2c_dev_t *dev, uint16_t reg_addr, uint8_t *out);
    HAL_StatusTypeDef i2c_read_16_bit(i2c_dev_t *dev, uint16_t reg_addr, uint16_t *out);
    HAL_StatusTypeDef i2c_write_8_bit(i2c_dev_t *dev, uint16_t reg_addr, uint8_t in);
    HAL_StatusTypeDef i2c_write_16_bit(i2c_dev_t *dev, uint16_t reg_addr, uint16_t in);
#ifdef __cplusplus
}
#endif

#endif