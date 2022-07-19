/**
 ******************************************************************************
 * @file           : as5600.h
 * @brief          : header file for AS5600 Encoder Library
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

#ifndef _AS5600_H
#define _AS5600_H

#include "i2c.h"
#include "ble_logger.h"
#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define AS5600_CHIP_ID 0x36

#define ZMCO 0x00
#define ZPOS_HI 0x01
#define ZPOS_LO 0x02
#define MPOS_HI 0x03
#define MPOS_LO 0x04
#define MANG_HI 0x05
#define MANG_LO 0x06
#define CONF_HI 0x07
#define CONF_LO 0x08
#define RAW_ANGLE_HI 0x0c
#define RAW_ANGLE_LO 0x0d
#define ANG_HI 0x0e
#define ANG_LO 0x0f
#define STAT 0x0b
#define AGC 0x1a
#define MAG_HI 0x1b
#define MAG_LO 0x1c
#define BURN 0xff

    // Configuration Register
    typedef enum
    {
        NOM = 0,
        LPM1 = 1,
        LPM2 = 2,
        LPM3 = 3
    } Power_Mode;

    typedef enum
    {
        OFF = 0,
        LSB_1 = 1,
        LSB_2 = 2,
        LSB_3 = 3
    } Hysteresis;

    typedef enum
    {
        ANALOG_1 = 0, // full range from 0% to 100% between GND and VDD
        ANALOG_2 = 1, // reduced range from 10% to 90% between GND and VDD,
        DIGITAL_PWM = 2
    } Output_Stage;

    typedef enum
    {
        FREQUENCY_115hz = 0,
        FREQUENCY_230hz = 1,
        FREQUENCY_460hz = 2,
        FREQUENCY_920hz = 3
    } PWM_Frequency;

    typedef enum
    {
        FILTER_16x = 0,
        FILTER_8x = 1,
        FILTER_4x = 2,
        FILTER_2x = 3
    } Slow_Filter;

    typedef enum
    {
        SLOW_FILTER = 0,
        LSB_6 = 1,
        LSB_7 = 2,
        LSB_9 = 3,
        LSB_18 = 4,
        LSB_21 = 5,
        LSB_24 = 6,
        LSB_10 = 7
    } Fast_Filter_Threshold;

    typedef enum
    {
        // OFF
        ON = 1
    } Watchdog;

    typedef struct
    {
        Power_Mode mode;
        Hysteresis hyst;
        Output_Stage out;
        PWM_Frequency freq;
        Slow_Filter filter;
        Fast_Filter_Threshold threshold;
        Watchdog wd;
    } as5600_config_t;

    HAL_StatusTypeDef as5600_init(I2C_TypeDef *instance, i2c_dev_t *dev);
    HAL_StatusTypeDef config_ams5600(i2c_dev_t *dev, as5600_config_t config);
    HAL_StatusTypeDef set_max_angle(i2c_dev_t *dev, uint16_t new_max_angle);
    HAL_StatusTypeDef get_max_angle(i2c_dev_t *dev, uint16_t *max_angle);

    HAL_StatusTypeDef set_start_position(i2c_dev_t *dev, uint16_t start_position);
    HAL_StatusTypeDef get_start_position(i2c_dev_t *dev, uint16_t *start_position);

    HAL_StatusTypeDef set_end_position(i2c_dev_t *dev, uint16_t end_angle);
    HAL_StatusTypeDef get_end_position(i2c_dev_t *dev, uint16_t *end_position);

    HAL_StatusTypeDef get_raw_angle(i2c_dev_t *dev, uint16_t *raw_angle);
    HAL_StatusTypeDef get_scaled_angle(i2c_dev_t *dev, uint16_t *scaled_angle);

    HAL_StatusTypeDef detect_magnet(i2c_dev_t *dev);

    HAL_StatusTypeDef get_agc(i2c_dev_t *dev, uint8_t *agc);
    HAL_StatusTypeDef get_magnitude(i2c_dev_t *dev, uint16_t *magnitude);
    HAL_StatusTypeDef get_conf(i2c_dev_t *dev, as5600_config_t *config);

    HAL_StatusTypeDef get_burn_count(i2c_dev_t *dev, uint8_t *count);

#ifdef __cplusplus
}
#endif

#endif