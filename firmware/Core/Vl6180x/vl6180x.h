#ifndef _VL6180x_H
#define _VL6180x_H

#include "stm32f4xx_hal.h"
#include "i2c.h"

// extern I2C_HandleTypeDef hi2c1;

#ifdef __cplusplus
extern "C"
{
#endif

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

    HAL_StatusTypeDef vl6180x_init(I2C_TypeDef *instance, i2c_dev_t *dev);

    HAL_StatusTypeDef vl6180x_configure(i2c_dev_t *dev);

    HAL_StatusTypeDef vl6180x_measure_distance(i2c_dev_t *dev, uint8_t *out_mm);

#ifdef __cplusplus
}
#endif

#endif