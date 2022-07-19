/**
 ******************************************************************************
 * @file           : ble_logger.h
 * @brief          : Source file for BLE logging library
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

#include "ble_logger.h"

UART_HandleTypeDef huart1;

HAL_StatusTypeDef ble_init(USART_TypeDef* instance)
{
    huart1.Instance = instance;
    huart1.Init.BaudRate = 9600;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    return HAL_UART_Init(&huart1);
}

void logger(stm32_log_level_t level, const char *TAG, const char *fmt, ...)
{
    char *log_print_buffer = calloc(LOG_BUFFER_SIZE, sizeof(char));

    switch (level)
    {
    case LOG_ERROR:
        sprintf(log_print_buffer, LOG_COLOR_E "%s: ", TAG);
        break;
    case LOG_WARN:
        sprintf(log_print_buffer, LOG_COLOR_W "%s: ", TAG);
        break;
    case LOG_INFO:
        sprintf(log_print_buffer, LOG_COLOR_I "%s: ", TAG);
        break;
    case LOG_DEBUG:
        sprintf(log_print_buffer, LOG_COLOR_D "%s: ", TAG);
        break;
    case LOG_VERBOSE:
        sprintf(log_print_buffer, LOG_COLOR_V "%s: ", TAG);
        break;
    default:
        break;
    }

    size_t fixed_length = strlen(log_print_buffer);

    va_list args;
    va_start(args, fmt);

    size_t variable_length = vsnprintf(NULL, 0, fmt, args) + 1;
    if (fixed_length + variable_length >= LOG_BUFFER_SIZE)
    {
        log_print_buffer = realloc(log_print_buffer, fixed_length + variable_length);
    }
    vsprintf(&log_print_buffer[fixed_length], fmt, args);

    va_end(args);
    HAL_UART_Transmit(&huart1, (uint8_t *)log_print_buffer, fixed_length + variable_length, TIMEOUT_MAX);
    free(log_print_buffer);
}