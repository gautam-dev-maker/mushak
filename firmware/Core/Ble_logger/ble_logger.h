/**
 ******************************************************************************
 * @file           : ble_logger.h
 * @brief          : header file for BLE logging library
 * @author         : Gautam Agrawal
 *
 * Created on: July 18, 2022
 ******************************************************************************
 * @attention
 *
 * Copyright (c)  2022 Society of Robotics and Automation
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 **/

#ifndef _BLE_LOGGER_H
#define _BLE_LOGGER_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f4xx_hal.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define LOG_BUFFER_SIZE 50

#define LOG_COLOR_BLACK "30"
#define LOG_COLOR_RED "31"
#define LOG_COLOR_GREEN "32"
#define LOG_COLOR_BROWN "33"
#define LOG_COLOR_BLUE "34"
#define LOG_COLOR_PURPLE "35"
#define LOG_COLOR_CYAN "36"
#define LOG_COLOR(COLOR) "\033[0;" COLOR "m"
#define LOG_BOLD(COLOR) "\033[1;" COLOR "m"
#define LOG_RESET_COLOR "\033[0m"
#define LOG_COLOR_E LOG_COLOR(LOG_COLOR_RED)
#define LOG_COLOR_W LOG_COLOR(LOG_COLOR_BROWN)
#define LOG_COLOR_I LOG_COLOR(LOG_COLOR_GREEN)
#define LOG_COLOR_D LOG_COLOR(LOG_COLOR_CYAN)
#define LOG_COLOR_V LOG_COLOR(LOG_COLOR_PURPLE)

#define TIMEOUT_MAX 100

    typedef enum
    {
        LOG_NONE,   /*!< No log output */
        LOG_ERROR,  /*!< Critical errors, software module can not recover on its own */
        LOG_WARN,   /*!< Error conditions from which recovery measures have been taken */
        LOG_INFO,   /*!< Information messages which describe normal flow of events */
        LOG_DEBUG,  /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
        LOG_VERBOSE /*!< Bigger chunks of debugging information, or frequent messages which can potentially flood the output. */
    } stm32_log_level_t;

#define BLE_LOG_V(TAG, fmt, ...) logger(LOG_VERBOSE, TAG, fmt, ##__VA_ARGS__);
#define BLE_LOG_D(TAG, fmt, ...) logger(LOG_DEBUG, TAG, fmt, ##__VA_ARGS__);
#define BLE_LOG_I(TAG, fmt, ...) logger(LOG_INFO, TAG, fmt, ##__VA_ARGS__);
#define BLE_LOG_W(TAG, fmt, ...) logger(LOG_WARN, TAG, fmt, ##__VA_ARGS__);
#define BLE_LOG_E(TAG, fmt, ...) logger(LOG_ERROR, TAG, fmt, ##__VA_ARGS__);

    HAL_StatusTypeDef ble_init(USART_TypeDef *instance);

    void logger(stm32_log_level_t level, const char *TAG, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif