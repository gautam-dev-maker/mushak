#ifndef __PWM_H__
#define __PWM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"

extern TIM_HandleTypeDef htim10;

HAL_StatusTypeDef pwm_init(void);

void pwm_set_duty_cycle(TIM_TypeDef *instance, uint32_t channel, uint8_t duty_cycle);

#ifdef __cplusplus
}
#endif

#endif

