#include "drv8833.h"

static TIM_HandleTypeDef *htim = NULL;

HAL_StatusTypeDef drv8833_init(TIM_HandleTypeDef *handler)
{
    htim = handler;
    TIM_ClockConfigTypeDef sClockSourceConfig = {0};
    TIM_MasterConfigTypeDef sMasterConfig = {0};
    TIM_OC_InitTypeDef sConfigOC = {0};
    TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

    /* USER CODE BEGIN TIM1_Init 1 */

    /* USER CODE END TIM1_Init 1 */
    htim->Instance = TIM1;
    htim->Init.Prescaler = 127;
    htim->Init.CounterMode = TIM_COUNTERMODE_UP;
    htim->Init.Period = 62499;
    htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim->Init.RepetitionCounter = 0;
    htim->Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(htim) != HAL_OK)
    {
        Error_Handler();
    }
    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    if (HAL_TIM_ConfigClockSource(htim, &sClockSourceConfig) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_PWM_Init(htim) != HAL_OK)
    {
        Error_Handler();
    }
    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    if (HAL_TIMEx_MasterConfigSynchronization(htim, &sMasterConfig) != HAL_OK)
    {
        Error_Handler();
    }
    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 31250;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
    sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    if (HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
    {
        Error_Handler();
    }
    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
    sBreakDeadTimeConfig.DeadTime = 0;
    sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
    if (HAL_TIMEx_ConfigBreakDeadTime(htim, &sBreakDeadTimeConfig) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN TIM1_Init 2 */

    /* USER CODE END TIM1_Init 2 */
    HAL_TIM_MspPostInit(htim);

    // Subject to change based on channels
    HAL_TIM_PWM_Start(htim, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(htim, TIM_CHANNEL_4);

    return HAL_OK;
}

HAL_StatusTypeDef set_motor_speed(int motor_id, int direction, float duty_cycle)
{
    unsigned int ARR = __HAL_TIM_GET_AUTORELOAD(htim);

    if (motor_id == MOTOR_A)
    {
        if (direction == MOTOR_FORWARD)
        {
            __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, (unsigned int)(ARR * duty_cycle));
        }
        else if (direction == MOTOR_BACKWARD)
        {
            __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, (unsigned int)(ARR * duty_cycle));
        }
        else
        {
            __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, 0);
            __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, 0);
        }
    }
    else if (motor_id == MOTOR_B)
    {
        if (direction == MOTOR_FORWARD)
        {
            __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, (unsigned int)(ARR * duty_cycle));
        }
        else if (direction == MOTOR_BACKWARD)
        {
            __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, (unsigned int)(ARR * duty_cycle));
        }
        else
        {
            __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, 0);
            __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, 0);
        }
    }
    return HAL_OK;
}