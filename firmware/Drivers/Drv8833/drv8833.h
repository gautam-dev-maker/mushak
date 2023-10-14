#ifndef __DRV8833_H__
#define __DRV8833_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

typedef enum {
    MOTOR_L = 0,
    MOTOR_R,
    MAX_MOTORS
} MOTOR;

typedef enum {
    BACKWARD = 0,
    FORWARD,
    STOP,
    MAX_MOTION
} MOTION;

HAL_StatusTypeDef drv8833_init();

void set_duty_cycle(MOTOR motor, MOTION motion, uint8_t duty_cycle);

void stop_motors();

#ifdef __cplusplus
}
#endif

#endif
