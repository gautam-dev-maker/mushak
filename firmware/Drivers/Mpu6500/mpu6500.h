#ifndef __MPU6500_H__
#define __MPU6500_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx_hal.h"

#define MPU6500_SPI hspi1
#define MPU6500_CS_GPIO GPIOA
#define MPU6500_CS_PIN GPIO_PIN_11

static const uint8_t READWRITE_CMD = 0x80;
static const uint8_t MULTIPLEBYTE_CMD = 0x40;
static const uint8_t DUMMY_BYTE = 0x00;

static const uint8_t _address = 0b11010000;
// 400 kHz
static const uint32_t _i2cRate = 400000;

// MPU6500 registers
static const uint8_t ACCEL_OUT = 0x3B;
static const uint8_t GYRO_OUT = 0x43;
static const uint8_t TEMP_OUT = 0x41;
static const uint8_t EXT_SENS_DATA_00 = 0x49;
static const uint8_t ACCEL_CONFIG = 0x1C;
static const uint8_t ACCEL_FS_SEL_2G = 0x00;
static const uint8_t ACCEL_FS_SEL_4G = 0x08;
static const uint8_t ACCEL_FS_SEL_8G = 0x10;
static const uint8_t ACCEL_FS_SEL_16G = 0x18;
static const uint8_t GYRO_CONFIG = 0x1B;
static const uint8_t GYRO_FS_SEL_250DPS = 0x00;
static const uint8_t GYRO_FS_SEL_500DPS = 0x08;
static const uint8_t GYRO_FS_SEL_1000DPS = 0x10;
static const uint8_t GYRO_FS_SEL_2000DPS = 0x18;
static const uint8_t ACCEL_CONFIG2 = 0x1D;
static const uint8_t DLPF_184 = 0x01;
static const uint8_t DLPF_92 = 0x02;
static const uint8_t DLPF_41 = 0x03;
static const uint8_t DLPF_20 = 0x04;
static const uint8_t DLPF_10 = 0x05;
static const uint8_t DLPF_5 = 0x06;
static const uint8_t CONFIG = 0x1A;
static const uint8_t SMPDIV = 0x19;
static const uint8_t INT_PIN_CFG = 0x37;
static const uint8_t INT_ENABLE = 0x38;
static const uint8_t INT_DISABLE = 0x00;
static const uint8_t INT_PULSE_50US = 0x00;
static const uint8_t INT_WOM_EN = 0x40;
static const uint8_t INT_RAW_RDY_EN = 0x01;
static const uint8_t PWR_MGMNT_1 = 0x6B;
static const uint8_t PWR_CYCLE = 0x20;
static const uint8_t PWR_RESET = 0x80;
static const uint8_t CLOCK_SEL_PLL = 0x01;
static const uint8_t PWR_MGMNT_2 = 0x6C;
static const uint8_t SEN_ENABLE = 0x00;
static const uint8_t DIS_GYRO = 0x07;
static const uint8_t USER_CTRL = 0x6A;
static const uint8_t I2C_MST_EN = 0x20;
static const uint8_t I2C_MST_CLK = 0x0D;
static const uint8_t I2C_MST_CTRL = 0x24;
static const uint8_t I2C_SLV0_ADDR = 0x25;
static const uint8_t I2C_SLV0_REG = 0x26;
static const uint8_t I2C_SLV0_DO = 0x63;
static const uint8_t I2C_SLV0_CTRL = 0x27;
static const uint8_t I2C_SLV0_EN = 0x80;
static const uint8_t I2C_READ_FLAG = 0x80;
static const uint8_t MOT_DETECT_CTRL = 0x69;
static const uint8_t ACCEL_INTEL_EN = 0x80;
static const uint8_t ACCEL_INTEL_MODE = 0x40;
static const uint8_t LP_ACCEL_ODR = 0x1E;
static const uint8_t WOM_THR = 0x1F;
static const uint8_t WHO_AM_I = 0x75;
static const uint8_t FIFO_EN = 0x23;
static const uint8_t FIFO_TEMP = 0x80;
static const uint8_t FIFO_GYRO = 0x70;
static const uint8_t FIFO_ACCEL = 0x08;
static const uint8_t FIFO_MAG = 0x01;
static const uint8_t FIFO_COUNT = 0x72;
static const uint8_t FIFO_READ = 0x74;

typedef enum GyroRange_
{
    GYRO_RANGE_250DPS = 0,
    GYRO_RANGE_500DPS,
    GYRO_RANGE_1000DPS,
    GYRO_RANGE_2000DPS
} GyroRange;

typedef enum AccelRange_
{
    ACCEL_RANGE_2G = 0,
    ACCEL_RANGE_4G,
    ACCEL_RANGE_8G,
    ACCEL_RANGE_16G
} AccelRange;

typedef enum DLPFBandwidth_
{
    DLPF_BANDWIDTH_184HZ = 0,
    DLPF_BANDWIDTH_92HZ,
    DLPF_BANDWIDTH_41HZ,
    DLPF_BANDWIDTH_20HZ,
    DLPF_BANDWIDTH_10HZ,
    DLPF_BANDWIDTH_5HZ
} DLPFBandwidth;

typedef enum SampleRateDivider_
{
    LP_ACCEL_ODR_0_24HZ = 0,
    LP_ACCEL_ODR_0_49HZ,
    LP_ACCEL_ODR_0_98HZ,
    LP_ACCEL_ODR_1_95HZ,
    LP_ACCEL_ODR_3_91HZ,
    LP_ACCEL_ODR_7_81HZ,
    LP_ACCEL_ODR_15_63HZ,
    LP_ACCEL_ODR_31_25HZ,
    LP_ACCEL_ODR_62_50HZ,
    LP_ACCEL_ODR_125HZ,
    LP_ACCEL_ODR_250HZ,
    LP_ACCEL_ODR_500HZ
} SampleRateDivider;

uint8_t mpu6500_init();
/* read the data, each argiment should point to a array for x, y, and x */
void mpu6500_get_data(int16_t *AccData, int16_t *GyroData);

/* sets the sample rate divider to values other than default */
void MPU6500_SetSampleRateDivider(SampleRateDivider srd);
/* sets the DLPF bandwidth to values other than default */
void mpu6500_set_dlpf_bandwidth(DLPFBandwidth bandwidth);
/* sets the gyro full scale range to values other than default */
void mpu6500_get_gyro_range(GyroRange range);
/* sets the accelerometer full scale range to values other than default */
void mpu6500_set_accel_range(AccelRange range);
#ifdef __cplusplus
}
#endif

#endif
