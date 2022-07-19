/**
 ******************************************************************************
 * @file           : vl6180x.h
 * @brief          : header file for VL6180x Library
 * @author         : Gautam Agrawal
 *
 * Created on: July 19, 2022
 ******************************************************************************
 * @attention
 *
 * Copyright (c)  2022 Society of Robotics and Automation
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 **/

#include "vl6180x.h"

HAL_StatusTypeDef vl6180x_init(I2C_TypeDef *instance, i2c_dev_t *dev) // I2C1
{
    dev->i2c_handle->Instance = instance;
    dev->i2c_handle->Init.ClockSpeed = 100000;
    dev->i2c_handle->Init.DutyCycle = I2C_DUTYCYCLE_2;
    dev->i2c_handle->Init.OwnAddress1 = 0;
    dev->i2c_handle->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    dev->i2c_handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    dev->i2c_handle->Init.OwnAddress2 = 0;
    dev->i2c_handle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    dev->i2c_handle->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    dev->dev_addr = VL6180X_DEFAULT_I2C_ADDR << 1; /* Since the address is 7 bits as per datasheet */

    return i2c_init(dev);
}

HAL_StatusTypeDef vl6180x_configure(i2c_dev_t *dev)
{
    i2c_write_8_bit(dev, 0x0207, 0x01);
    i2c_write_8_bit(dev, 0x0208, 0x01);
    i2c_write_8_bit(dev, 0x0096, 0x00);
    i2c_write_8_bit(dev, 0x0097, 0xfd);
    i2c_write_8_bit(dev, 0x00e3, 0x00);
    i2c_write_8_bit(dev, 0x00e4, 0x04);
    i2c_write_8_bit(dev, 0x00e5, 0x02);
    i2c_write_8_bit(dev, 0x00e6, 0x01);
    i2c_write_8_bit(dev, 0x00e7, 0x03);
    i2c_write_8_bit(dev, 0x00f5, 0x02);
    i2c_write_8_bit(dev, 0x00d9, 0x05);
    i2c_write_8_bit(dev, 0x00db, 0xce);
    i2c_write_8_bit(dev, 0x00dc, 0x03);
    i2c_write_8_bit(dev, 0x00dd, 0xf8);
    i2c_write_8_bit(dev, 0x009f, 0x00);
    i2c_write_8_bit(dev, 0x00a3, 0x3c);
    i2c_write_8_bit(dev, 0x00b7, 0x00);
    i2c_write_8_bit(dev, 0x00bb, 0x3c);
    i2c_write_8_bit(dev, 0x00b2, 0x09);
    i2c_write_8_bit(dev, 0x00ca, 0x09);
    i2c_write_8_bit(dev, 0x0198, 0x01);
    i2c_write_8_bit(dev, 0x01b0, 0x17);
    i2c_write_8_bit(dev, 0x01ad, 0x00);
    i2c_write_8_bit(dev, 0x00ff, 0x05);
    i2c_write_8_bit(dev, 0x0100, 0x05);
    i2c_write_8_bit(dev, 0x0199, 0x05);
    i2c_write_8_bit(dev, 0x01a6, 0x1b);
    i2c_write_8_bit(dev, 0x01ac, 0x3e);
    i2c_write_8_bit(dev, 0x01a7, 0x1f);
    i2c_write_8_bit(dev, 0x0030, 0x00);

    /* Recommended _ Public registers - See data sheet for more detail */
    i2c_write_8_bit(dev, 0x0011, 0x10);        /* Enables polling for 'New Sample ready' when measurement completes */
    i2c_write_8_bit(dev, 0x010a, 0x30);        /* Set the averaging sample period (compromise between lower noise and increased execution time) */
    i2c_write_8_bit(dev, 0x003f, 0x46);        /* Sets the light and dark gain (upper nibble). Dark gain should not be changed */
    i2c_write_8_bit(dev, 0x0031, 0xFF);        /* sets the # of range measurements after which auto calibration of system is performed */
    i2c_write_8_bit(dev, 0x0040, 0x63);        /* Set ALS integration time to 100ms */
    return i2c_write_8_bit(dev, 0x002e, 0x01); /* perform a single temperature calibration of the ranging sensor */
}

HAL_StatusTypeDef vl6180x_measure_distance(i2c_dev_t *dev, uint8_t *range)
{
    uint8_t status;
    while (!(status & 0x01))
    {
        i2c_read_8_bit(dev, VL6180X_REG_RESULT_RANGE_STATUS, &status);
    }

    // Start a range measurement
    i2c_write_8_bit(dev, VL6180X_REG_SYSRANGE_START, 0x01);

    // Poll until bit 2 is set (not sure about this)
    while (!(status & 0x04))
    {
        i2c_read_8_bit(dev, VL6180X_REG_RESULT_INTERRUPT_STATUS_GPIO, &status);
    }

    // read range in mm
    i2c_read_8_bit(dev, VL6180X_REG_RESULT_RANGE_VAL, range);

    // clear interrupt
    return i2c_write_8_bit(dev, VL6180X_REG_SYSTEM_INTERRUPT_CLEAR, 0x07);
}
