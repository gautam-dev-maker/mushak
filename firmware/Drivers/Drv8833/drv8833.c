#include "drv8833.h"
#include "pwm.h"

#define DRV8833_EN_Pin GPIO_PIN_2
#define DRV8833_EN_GPIO_Port GPIOB

HAL_StatusTypeDef drv8833_init()
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    HAL_GPIO_WritePin(DRV8833_EN_GPIO_Port, DRV8833_EN_Pin, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = DRV8833_EN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DRV8833_EN_GPIO_Port, &GPIO_InitStruct);
    HAL_GPIO_WritePin(DRV8833_EN_GPIO_Port, DRV8833_EN_Pin, GPIO_PIN_SET);
    return pwm_init();
}

void set_duty_cycle(MOTOR motor, MOTION motion, uint8_t duty_cycle)
{
    switch (motor) {
        case MOTOR_L:
            switch (motion) {
                case BACKWARD:
                    pwm_set_duty_cycle(TIM10, TIM_CHANNEL_1, duty_cycle);
                    pwm_set_duty_cycle(TIM3, TIM_CHANNEL_3, 0);
                    break;
                case FORWARD:
                    pwm_set_duty_cycle(TIM10, TIM_CHANNEL_1, 0);
                    pwm_set_duty_cycle(TIM3, TIM_CHANNEL_3, duty_cycle);
                    break;
                case STOP:
                    pwm_set_duty_cycle(TIM10, TIM_CHANNEL_1, 0);
                    pwm_set_duty_cycle(TIM3, TIM_CHANNEL_3, 0);
                    break;
                default:
                    break;
            }
            break;
        case MOTOR_R:
            switch (motion) {
                case BACKWARD:
                    pwm_set_duty_cycle(TIM3, TIM_CHANNEL_4, duty_cycle);
                    pwm_set_duty_cycle(TIM13, TIM_CHANNEL_1, 0);
                    break;
                case FORWARD:
                    pwm_set_duty_cycle(TIM3, TIM_CHANNEL_4, 0);
                    pwm_set_duty_cycle(TIM13, TIM_CHANNEL_1, duty_cycle);
                    break;
                case STOP:
                    pwm_set_duty_cycle(TIM3, TIM_CHANNEL_4, 0);
                    pwm_set_duty_cycle(TIM13, TIM_CHANNEL_1, 0);
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    HAL_GPIO_WritePin(DRV8833_EN_GPIO_Port, DRV8833_EN_Pin, GPIO_PIN_SET);
}

void stop_motors()
{
    HAL_GPIO_WritePin(DRV8833_EN_GPIO_Port, DRV8833_EN_Pin, GPIO_PIN_RESET);
}
