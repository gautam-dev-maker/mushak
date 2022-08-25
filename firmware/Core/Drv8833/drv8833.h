#ifndef DRV8833_H
#define DRV8833_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "main.h"
#include "stm32f4xx_hal.h"

#define MOTOR_FORWARD 200
#define MOTOR_BACKWARD 201
#define MOTOR_STOP 202

#define MOTOR_A 100
#define MOTOR_B 102

HAL_StatusTypeDef drv8833_init(TIM_HandleTypeDef *htim);

HAL_StatusTypeDef set_motor_speed(int motor_id, int direction, float duty_cycle);

#endif