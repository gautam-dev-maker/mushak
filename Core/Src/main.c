/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "main.h"
#include <string.h>
#include <stdio.h>

#define VL6180X_WRITE_ADDR 0x52
#define VL6180X_READ_ADDR 0x53
#define I2C_TIMEOUT 50
/* The fixed I2C address of the device */
#define VL6180X_DEFAULT_I2C_ADDR 0x29
/* Device model identification number */
#define VL6180X_REG_IDENTIFICATION_MODEL_ID 0x000
/* Interrupt configuration */
#define VL6180X_REG_SYSTEM_INTERRUPT_CONFIG 0x014
/* Interrupt clear bits */
#define VL6180X_REG_SYSTEM_INTERRUPT_CLEAR 0x015
/* Fresh out of reset bit */
#define VL6180X_REG_SYSTEM_FRESH_OUT_OF_RESET 0x016
/* Trigger Ranging */
#define VL6180X_REG_SYSRANGE_START 0x018
/* Trigger Lux Reading */
#define VL6180X_REG_SYSALS_START 0x038
/* Lux reading gain */
#define VL6180X_REG_SYSALS_ANALOGUE_GAIN 0x03F
/* Integration period for ALS mode, high byte */
#define VL6180X_REG_SYSALS_INTEGRATION_PERIOD_HI 0x040
/* Integration period for ALS mode, low byte */
#define VL6180X_REG_SYSALS_INTEGRATION_PERIOD_LO 0x041
/* Specific error codes */
#define VL6180X_REG_RESULT_RANGE_STATUS 0x04d
/* Interrupt status */
#define VL6180X_REG_RESULT_INTERRUPT_STATUS_GPIO 0x04f
/* Light reading value */
#define VL6180X_REG_RESULT_ALS_VAL 0x050
/* Ranging reading value */
#define VL6180X_REG_RESULT_RANGE_VAL 0x062
/* I2C Slave Device Address */
#define VL6180X_REG_SLAVE_DEVICE_ADDRESS 0x212
/* period between each measurement when in continuous mode */
#define SYSRANGE_INTERMEASUREMENT_PERIOD 0x001b // P19 application notes

#define VL6180X_ALS_GAIN_1 0x06    /* 1x gain */
#define VL6180X_ALS_GAIN_1_25 0x05 /* 1.25x gain */
#define VL6180X_ALS_GAIN_1_67 0x04 /* 1.67x gain */
#define VL6180X_ALS_GAIN_2_5 0x03  /* 2.5x gain */
#define VL6180X_ALS_GAIN_5 0x02    /* 5x gain */
#define VL6180X_ALS_GAIN_10 0x01   /* 10x gain */
#define VL6180X_ALS_GAIN_20 0x00   /* 20x gain */
#define VL6180X_ALS_GAIN_40 0x07   /* 40x gain */

#define VL6180X_ERROR_NONE 0        /* Success! */
#define VL6180X_ERROR_SYSERR_1 1    /* System error */
#define VL6180X_ERROR_SYSERR_5 5    /* Sysem error */
#define VL6180X_ERROR_ECEFAIL 6     /* Early convergence estimate fail */
#define VL6180X_ERROR_NOCONVERGE 7  /* No target detected */
#define VL6180X_ERROR_RANGEIGNORE 8 /* Ignore threshold check failed */
#define VL6180X_ERROR_SNR 11        /* Ambient conditions too high */
#define VL6180X_ERROR_RAWUFLOW 12   /* Raw range algo underflow */
#define VL6180X_ERROR_RAWOFLOW 13   /* Raw range algo overflow */
#define VL6180X_ERROR_RANGEUFLOW 14 /* Raw range algo underflow */
#define VL6180X_ERROR_RANGEOFLOW 15 /* Raw range algo overflow */

#define VL6180X_REGISTERS_HVL6180X_REGISTERS_H

#define VL6180X_SYSTEM_INTERRUPT_CLEAR 0x15

#define VL6180X_SYSRANGE_START 0x18

#define VL6180X_RESULT_RANGE_STATUS 0x4d
#define VL6180X_RESULT_INTERRUPT_STATUS_GPIO 0x4f
#define VL6180X_RESULT_RANGE_VAL 0x62

I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
void vl6180x_configure(void);
void i2c_read_8_bit(uint16_t reg_addr, uint8_t *out);
void i2c_read_16_bit(uint16_t reg_addr, uint16_t *out);
void i2c_write_8_bit(uint16_t reg_addr, uint8_t in);
void i2c_write_16_bit(uint16_t reg_addr, uint16_t in);
void vl6180x_measure_distance(uint8_t *out_mm);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  uint8_t val = 100;
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  vl6180x_configure();

  while (1)
  {
    uint8_t output[5];
    vl6180x_measure_distance(&val);
    sprintf(output, "%u \n" , val);
    HAL_UART_Transmit(&huart1, output, sizeof(output), 50);
    HAL_Delay(100) ;
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

void i2c_read_8_bit(uint16_t reg_addr, uint8_t *out)
{
  uint8_t data[2] = {reg_addr >> 8, reg_addr};
  HAL_I2C_Master_Transmit(&hi2c1, VL6180X_WRITE_ADDR, data, 2, I2C_TIMEOUT);
  HAL_I2C_Master_Receive(&hi2c1, VL6180X_READ_ADDR, out, 1, I2C_TIMEOUT);
}

void i2c_read_16_bit(uint16_t reg_addr, uint16_t *out)
{
  uint8_t data[2] = {reg_addr >> 8, reg_addr};
  uint8_t out_data[2] = {0,0};
  HAL_I2C_Master_Transmit(&hi2c1, VL6180X_WRITE_ADDR, data, 2, I2C_TIMEOUT);
  HAL_I2C_Master_Receive(&hi2c1, VL6180X_READ_ADDR, out_data, 2, I2C_TIMEOUT);
  *out = out_data[0];
  *out <<= 8;
  *out |= out_data[1];
}

void i2c_write_8_bit(uint16_t reg_addr, uint8_t in)
{
    uint8_t data[3] = {reg_addr >> 8, reg_addr, in};
    HAL_I2C_Master_Transmit(&hi2c1, VL6180X_WRITE_ADDR, data, 3, I2C_TIMEOUT);
}

void i2c_write_16_bit(uint16_t reg_addr, uint16_t in)
{
    uint8_t data[4] = {reg_addr >> 8, reg_addr, in >> 8, in};
    HAL_I2C_Master_Transmit(&hi2c1, VL6180X_WRITE_ADDR, data, 4, I2C_TIMEOUT);
}

void vl6180x_configure(void)
{
  uint8_t val;
  i2c_read_8_bit(0x16, &val);
    while (val != 0x01) {
    }

    /* Mandatory: Private registers. */
    i2c_write_8_bit(0x0207, 0x01);
    i2c_write_8_bit(0x0208, 0x01);
    i2c_write_8_bit(0x0096, 0x00);
    i2c_write_8_bit(0x0097, 0xfd);
    i2c_write_8_bit(0x00e3, 0x00);
    i2c_write_8_bit(0x00e4, 0x04);
    i2c_write_8_bit(0x00e5, 0x02);
    i2c_write_8_bit(0x00e6, 0x01);
    i2c_write_8_bit(0x00e7, 0x03);
    i2c_write_8_bit(0x00f5, 0x02);
    i2c_write_8_bit(0x00d9, 0x05);
    i2c_write_8_bit(0x00db, 0xce);
    i2c_write_8_bit(0x00dc, 0x03);
    i2c_write_8_bit(0x00dd, 0xf8);
    i2c_write_8_bit(0x009f, 0x00);
    i2c_write_8_bit(0x00a3, 0x3c);
    i2c_write_8_bit(0x00b7, 0x00);
    i2c_write_8_bit(0x00bb, 0x3c);
    i2c_write_8_bit(0x00b2, 0x09);
    i2c_write_8_bit(0x00ca, 0x09);
    i2c_write_8_bit(0x0198, 0x01);
    i2c_write_8_bit(0x01b0, 0x17);
    i2c_write_8_bit(0x01ad, 0x00);
    i2c_write_8_bit(0x00ff, 0x05);
    i2c_write_8_bit(0x0100, 0x05);
    i2c_write_8_bit(0x0199, 0x05);
    i2c_write_8_bit(0x01a6, 0x1b);
    i2c_write_8_bit(0x01ac, 0x3e);
    i2c_write_8_bit(0x01a7, 0x1f);
    i2c_write_8_bit(0x0030, 0x00);

    /* Recommended : Public registers - See data sheet for more detail */

    /* Enables polling for New Sample ready when measurement completes */
    i2c_write_8_bit(0x0011, 0x10);
    /* Set the averaging sample period (compromise between lower noise and
     * increased execution time) */
    i2c_write_8_bit(0x010a, 0x30);
    /* Sets the light and dark gain (upper nibble). Dark gain should not be
     * changed.*/
    i2c_write_8_bit(0x003f, 0x46);
    /* sets the # of range measurements after which auto calibration of system
     * is performed */
    i2c_write_8_bit(0x0031, 0xFF);
    /* Set ALS integration time to 100ms */
    i2c_write_8_bit(0x0040, 0x63);
    /* perform a single temperature calibration of the ranging sensor */
    i2c_write_8_bit(0x002e, 0x01);
}

void vl6180x_measure_distance(uint8_t *out_mm)
{
    uint8_t status, mm;

    /* Wait for device ready. */
    do {
        i2c_read_8_bit(VL6180X_RESULT_RANGE_STATUS, &status);
    } while ((status & (1 << 0)) == 0);

    /* Start measurement. */
    i2c_write_8_bit(VL6180X_SYSRANGE_START, 0x01);

    /* Wait for measurement ready. */
    // do {
    //     status = i2c_read_8_bit(VL6180X_RESULT_INTERRUPT_STATUS_GPIO);
    // } while ((status & (1 << 2)) == 0);

    /* Read result. */
    i2c_read_8_bit(VL6180X_RESULT_RANGE_VAL, &mm);
    *out_mm = mm;

    /* Clear interrupt flags. */
    i2c_write_8_bit(VL6180X_SYSTEM_INTERRUPT_CLEAR, 0x07);

    /* Wait for device ready. */
    do {
        i2c_read_8_bit(VL6180X_RESULT_RANGE_STATUS, &status);
    } while ((status & (1 << 0)) == 0);
}
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
