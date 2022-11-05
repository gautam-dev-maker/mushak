/**
 ******************************************************************************
 * @file           : adc.h
 * @brief          : header file for ADC library
 * @author         : Gautam Agrawal
 *
 * Created on: November 2, 2022
 ******************************************************************************
 * @attention
 *
 * Copyright (c)  2022 Society of Robotics and Automation
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 **/

#ifndef _ADC_H
#define _ADC_H

#include "stdio.h"
#include "stm32f4xx_hal.h"

HAL_StatusTypeDef adc_init(void);
uint32_t get_adc_readings(uint32_t channel);

#endif