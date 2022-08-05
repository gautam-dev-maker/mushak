/**
 ******************************************************************************
 * @file           : as5600.c
 * @brief          : source code for AS5600 Encoder Library
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

#include "as5600.h"

#define AS5600 "AS5600"

HAL_StatusTypeDef as5600_init(I2C_TypeDef *instance, i2c_dev_t *dev)
{
    dev->i2c_handle.Instance = instance;
    dev->i2c_handle.Init.ClockSpeed = 100000;
    dev->i2c_handle.Init.DutyCycle = I2C_DUTYCYCLE_2;
    dev->i2c_handle.Init.OwnAddress1 = 0;
    dev->i2c_handle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    dev->i2c_handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    dev->i2c_handle.Init.OwnAddress2 = 0;
    dev->i2c_handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    dev->i2c_handle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

    dev->dev_addr = AS5600_CHIP_ID << 1; /* Since the address is 7 bits as per datasheet */

    return i2c_init(dev);
}

HAL_StatusTypeDef config_ams5600(i2c_dev_t *dev, as5600_config_t config)
{
    uint8_t config_lo = config.freq << 6 | config.out << 4 | config.hyst << 2 | config.mode;
    uint8_t config_hi = config.wd << 5 | config.threshold << 2 | config.filter;

    HAL_StatusTypeDef ret = i2c_write_8_bit(dev, CONF_LO, config_lo);
    HAL_StatusTypeDef ret2 = i2c_write_8_bit(dev, CONF_HI, config_hi);
    return ret && ret2;
}

HAL_StatusTypeDef set_max_angle(i2c_dev_t *dev, uint16_t new_max_angle)
{
    uint8_t low_byte = new_max_angle & 0xff;
    uint8_t high_byte = (new_max_angle >> 8) & 0xff;

    HAL_StatusTypeDef ret1 = i2c_write_8_bit(dev, MANG_LO, low_byte);
    HAL_StatusTypeDef ret2 = i2c_write_8_bit(dev, MANG_HI, high_byte);

    return ret1 && ret2;
}

HAL_StatusTypeDef get_max_angle(i2c_dev_t *dev, uint16_t *max_angle)
{
    uint8_t high, low;
    HAL_StatusTypeDef ret1 = i2c_read_8_bit(dev, MANG_HI, &high);
    HAL_StatusTypeDef ret2 = i2c_read_8_bit(dev, MANG_LO, &low);
    *max_angle = ((high << 8) | low);
    return ret1 && ret2;
}

HAL_StatusTypeDef set_start_position(i2c_dev_t *dev, uint16_t start_position)
{
    uint8_t low_byte = start_position & 0xff;
    uint8_t high_byte = (start_position >> 8) & 0xff;

    HAL_StatusTypeDef ret1 = i2c_write_8_bit(dev, ZPOS_LO, low_byte);
    HAL_StatusTypeDef ret2 = i2c_write_8_bit(dev, ZPOS_HI, high_byte);

    return ret1 && ret2;
}

HAL_StatusTypeDef get_start_position(i2c_dev_t *dev, uint16_t *start_position)
{
    uint8_t high, low;
    HAL_StatusTypeDef ret1 = i2c_read_8_bit(dev, ZPOS_HI, &high);
    HAL_StatusTypeDef ret2 = i2c_read_8_bit(dev, ZPOS_LO, &low);
    *start_position = ((high << 8) | low);
    return ret1 && ret2;
}

HAL_StatusTypeDef set_end_position(i2c_dev_t *dev, uint16_t end_angle)
{
    uint8_t low_byte = end_angle & 0xff;
    uint8_t high_byte = (end_angle >> 8) & 0xff;

    HAL_StatusTypeDef ret1 = i2c_write_8_bit(dev, MPOS_LO, low_byte);
    HAL_StatusTypeDef ret2 = i2c_write_8_bit(dev, MPOS_HI, high_byte);

    return ret1 && ret2;
}

HAL_StatusTypeDef get_end_position(i2c_dev_t *dev, uint16_t *end_position)
{
    uint8_t high, low;
    HAL_StatusTypeDef ret1 = i2c_read_8_bit(dev, MPOS_HI, &high);
    HAL_StatusTypeDef ret2 = i2c_read_8_bit(dev, MPOS_LO, &low);
    *end_position = ((high << 8) | low);
    return ret1 && ret2;
}

HAL_StatusTypeDef get_raw_angle(i2c_dev_t *dev, uint16_t *raw_angle)
{
    uint8_t high, low;
    HAL_StatusTypeDef ret1 = i2c_read_8_bit(dev, RAW_ANGLE_HI, &high);
    HAL_StatusTypeDef ret2 = i2c_read_8_bit(dev, RAW_ANGLE_LO, &low);
    *raw_angle = ((high << 8) | low);
    return ret1 && ret2;
}

HAL_StatusTypeDef get_scaled_angle(i2c_dev_t *dev, uint16_t *scaled_angle)
{
    uint8_t high, low;
    HAL_StatusTypeDef ret1 = i2c_read_8_bit(dev, ANG_HI, &high);
    HAL_StatusTypeDef ret2 = i2c_read_8_bit(dev, ANG_LO, &low);
    *scaled_angle = ((high << 8) | low);
    return ret1 && ret2;
}

HAL_StatusTypeDef detect_magnet(i2c_dev_t *dev)
{
    uint8_t mag_status;

    if (i2c_read_8_bit(dev, STAT, &mag_status) != HAL_OK)
    {
        BLE_LOG_I(AS5600, "Unable to Read Status Register\n");
        return HAL_ERROR;
    }

    if (mag_status & 0x20)
    {
        BLE_LOG_I(AS5600, "Magnet was detected\n");
        return HAL_OK;
    }
    else if (mag_status & 0x10)
    {
        BLE_LOG_I(AS5600, "AGC maximum gain overflow, magnet too weak\n");
        return HAL_ERROR;
    }
    else if (mag_status & 0x08)
    {
        BLE_LOG_I(AS5600, "AGC minimum gain overflow, magnet too strong\n");
        return HAL_ERROR;
    }
    return HAL_ERROR;
}

HAL_StatusTypeDef get_agc(i2c_dev_t *dev, uint8_t *agc)
{
    return i2c_read_8_bit(dev, AGC, agc);
}

HAL_StatusTypeDef get_magnitude(i2c_dev_t *dev, uint16_t *magnitude)
{
    uint8_t high, low;
    HAL_StatusTypeDef ret1 = i2c_read_8_bit(dev, MAG_HI, &high);
    HAL_StatusTypeDef ret2 = i2c_read_8_bit(dev, MAG_LO, &low);
    *magnitude = ((high << 8) | low);
    return ret1 && ret2;
}

HAL_StatusTypeDef get_conf(i2c_dev_t *dev, as5600_config_t *config)
{
    uint8_t high, low;
    HAL_StatusTypeDef ret = i2c_read_8_bit(dev, CONF_HI, &high);
    HAL_StatusTypeDef ret1 = i2c_read_8_bit(dev, CONF_LO, &low);

    config->mode = (low & 0x03);
    config->hyst = ((low & 0x0c) >> 2);
    config->out = ((low & 0x30) >> 4);
    config->freq = ((low & 0xc0) >> 6);
    config->filter = ((high & 0x03));
    config->threshold = ((high & 0x1c)) >> 2;
    config->wd = ((high & 0x20)) >> 5;

    return ret & ret1;
}

HAL_StatusTypeDef get_burn_count(i2c_dev_t *dev, uint8_t *count)
{
    return i2c_read_8_bit(dev, ZMCO, count);
}
