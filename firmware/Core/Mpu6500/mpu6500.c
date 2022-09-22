#include "mpu6500.h"
#include "ble_logger.h"

static SPI_HandleTypeDef hspi1;

static uint8_t _buffer[14];
// static uint8_t _mag_adjust[3];

__weak void MPU6500_OnActivate()
{
}

static inline void MPU6500_Activate()
{
    MPU6500_OnActivate();
    HAL_GPIO_WritePin(MPU6500_CS_GPIO, MPU6500_CS_PIN, GPIO_PIN_RESET);
}

static inline void MPU6500_Deactivate()
{
    HAL_GPIO_WritePin(MPU6500_CS_GPIO, MPU6500_CS_PIN, GPIO_PIN_SET);
}

uint8_t SPIx_WriteRead(uint8_t Byte)
{
    uint8_t receivedbyte = 0;
    if (HAL_SPI_TransmitReceive(&hspi1, (uint8_t *)&Byte, (uint8_t *)&receivedbyte, 1, 0x1000) != HAL_OK)
    {
        return -1;
    }
    else
    {
    }
    return receivedbyte;
}

void MPU_SPI_Write(uint8_t *pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
    MPU6500_Activate();
    SPIx_WriteRead(WriteAddr);
    while (NumByteToWrite >= 0x01)
    {
        SPIx_WriteRead(*pBuffer);
        NumByteToWrite--;
        pBuffer++;
    }
    MPU6500_Deactivate();
}

void MPU_SPI_Read(uint8_t *pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{
    MPU6500_Activate();
    uint8_t data = ReadAddr | READWRITE_CMD;
    HAL_SPI_Transmit(&MPU6500_SPI, &data, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&MPU6500_SPI, pBuffer, NumByteToRead, HAL_MAX_DELAY);
    MPU6500_Deactivate();
}

/* writes a byte to MPU6500 register given a register address and data */
void writeRegister(uint8_t subAddress, uint8_t data)
{
    MPU_SPI_Write(&data, subAddress, 1);
    HAL_Delay(10);
}

/* reads registers from MPU6500 given a starting register address, number of bytes, and a pointer to store data */
void readRegisters(uint8_t subAddress, uint8_t count, uint8_t *dest)
{
    MPU_SPI_Read(dest, subAddress, count);
}

/* gets the MPU6500 WHO_AM_I register value, expected to be 0x71 */
static uint8_t whoAmI()
{
    // read the WHO AM I register
    readRegisters(WHO_AM_I, 1, _buffer);

    // return the register value
    return _buffer[0];
}

/* starts communication with the MP6500 */
uint8_t MPU6500_Init()
{
    /* Initialising the GPIO of Chip select */
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    HAL_GPIO_WritePin(MPU6500_CS_GPIO, MPU6500_CS_PIN, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = MPU6500_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(MPU6500_CS_GPIO, &GPIO_InitStruct);

    /* Initialising the SPI Instance */

    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
    hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 10;
    HAL_SPI_Init(&hspi1);

    // select clock source to gyro
    writeRegister(PWR_MGMNT_1, CLOCK_SEL_PLL);
    // enable I2C master mode
    writeRegister(USER_CTRL, I2C_MST_EN);
    // set the I2C bus speed to 400 kHz
    writeRegister(I2C_MST_CTRL, I2C_MST_CLK);

    // reset the MPU6500
    writeRegister(PWR_MGMNT_1, PWR_RESET);
    // wait for MPU-6500 to come back up
    HAL_Delay(10);

    // select clock source to gyro
    writeRegister(PWR_MGMNT_1, CLOCK_SEL_PLL);

    // check the WHO AM I byte, expected value is 0x71 (decimal 113) or 0x73 (decimal 115)
    uint8_t who = whoAmI();
    if ((who != 0x71) && (who != 0x73))
    {
        return 1;
    }

    // enable accelerometer and gyro
    writeRegister(PWR_MGMNT_2, SEN_ENABLE);

    // setting accel range to 16G as default
    writeRegister(ACCEL_CONFIG, ACCEL_FS_SEL_16G);

    // setting the gyro range to 2000DPS as default
    writeRegister(GYRO_CONFIG, GYRO_FS_SEL_250DPS);

    // setting bandwidth to 184Hz as default
    writeRegister(ACCEL_CONFIG2, DLPF_184);

    // setting gyro bandwidth to 184Hz
    writeRegister(CONFIG, DLPF_184);

    // setting the sample rate divider to 0 as default
    writeRegister(SMPDIV, 0x00);

    // enable I2C master mode
    writeRegister(USER_CTRL, I2C_MST_EN);

    // set the I2C bus speed to 400 kHz
    writeRegister(I2C_MST_CTRL, I2C_MST_CLK);

    // select clock source to gyro
    writeRegister(PWR_MGMNT_1, CLOCK_SEL_PLL);

    // successful init, return 0
    return 0;
}

/* sets the accelerometer full scale range to values other than default */
void MPU6500_SetAccelRange(AccelRange range)
{
    writeRegister(ACCEL_CONFIG, range);
}

/* sets the gyro full scale range to values other than default */
void MPU6500_SetGyroRange(GyroRange range)
{
    writeRegister(GYRO_CONFIG, range);
}

/* sets the DLPF bandwidth to values other than default */
void MPU6500_SetDLPFBandwidth(DLPFBandwidth bandwidth)
{
    writeRegister(ACCEL_CONFIG2, bandwidth);
    writeRegister(CONFIG, bandwidth);
}

void compute_acce_angle(int16_t ax, int16_t ay, int16_t az, double *acce_angle)
{
    // acce_angle[0] = atan2(ay, sqrt(pow(ax, 2) + pow(az, 2))) * RAD_TO_DEG;
    // acce_angle[1] = atan2(-ax, sqrt(pow(ay, 2) + pow(az, 2))) * RAD_TO_DEG;
    acce_angle[0] = atan2(ay, az) * RAD_TO_DEG - 6.0;
    acce_angle[1] = atan2(ax, az) * RAD_TO_DEG - 14.0;
    BLE_LOG_I("MPU6500 acc x", "%.3lf\n", (acce_angle[0]));
    BLE_LOG_I("MPU6500 acc y", "%.3lf\n", (acce_angle[1]));
    BLE_LOG_I("MPU6500 acc z", "%.3lf\n", (acce_angle[2]));
}

// Compute the gyroscope angle using the raw data
void compute_gyro_angle(int16_t gx, int16_t gy, int16_t gz, uint32_t dt, double *gyro_angle)
{
    // (1 / 131) sensitivity factor of Gyroscope: 1 degree rotation gives a reading of 131 units
    // static float g_x = 0, g_y = 0, g_z = 0;
    // g_x = g_x + ((float)gx * (float)dt) / 131.0;
    // g_y = g_y + ((float)gy * (float)dt) / 131.0;
    // g_z = g_z + ((float)gz * (float)dt) / 131.0;
    gyro_angle[0] = (((double)gx * (double)dt) / 131.0) - 8;
    gyro_angle[1] = (((double)gy * (double)dt) / 131.0) + 2;
    gyro_angle[2] = ((double)gz * (double)dt) / 131.0;
    BLE_LOG_I("MPU6500 gyro x", "%.3lf\n", (gyro_angle[0]));
    BLE_LOG_I("MPU6500 gyro y", "%.3lf\n", (gyro_angle[1]));
    BLE_LOG_I("MPU6500 gyro z", "%.3lf\n", (gyro_angle[2]));
    /*
        In case the roll angle varies widely when only the pitch angle changes, activate the following equations.
        gyro_angle[0] = dt * (gx + gy * sin(gyro_angle[0]) * tan(gyro_angle[1]) + gz * cos(gyro_angle[0]) * tan(gyro_angle[1]));
        gyro_angle[1] = dt * (gy * cos(gyro_angle[0]) - gz * sin(gyro_angle[0]));
    */
}

void MPU6500_ComplementaryFilter(int16_t *acc_raw_val, int16_t *gyro_raw_val, double *complementary_angle, float *mpu_offset)
{

    static uint8_t is_initial_reading = 0;
    double acce_angle[2];
    double gyro_angle[3];
    uint16_t dt;
    HAL_InitTick(15);
    static uint32_t timer;

    if (is_initial_reading == 0)
    {
        is_initial_reading++;
        compute_acce_angle(acc_raw_val[0], acc_raw_val[1], acc_raw_val[2], acce_angle);

        for (int i = 0; i < 2; i++)
            complementary_angle[i] = acce_angle[i] - mpu_offset[i];
        complementary_angle[2] = 0;

        timer = HAL_GetTick() / 1000;
        return;
    }
    dt = (HAL_GetTick() / 1000) - timer;
    timer = HAL_GetTick() / 1000;

    compute_acce_angle(acc_raw_val[0], acc_raw_val[1], acc_raw_val[2], acce_angle);
    compute_gyro_angle(gyro_raw_val[0], gyro_raw_val[1], gyro_raw_val[2], dt, gyro_angle);

    for (int i = 0; i < 2; i++)
    {
        acce_angle[i] = acce_angle[i] - mpu_offset[i];
        complementary_angle[i] = ALPHA * (gyro_angle[i]) + (1 - ALPHA) * acce_angle[i];
    }
    complementary_angle[2] = gyro_angle[2];
    BLE_LOG_I("MPU6500 comp x", "%.3lf\n", (complementary_angle[0]));
    BLE_LOG_I("MPU6500 comp y", "%.3lf\n", (complementary_angle[1]));
    BLE_LOG_I("MPU6500 comp z", "%.3lf\n", (complementary_angle[2]));
}

/* read the data, each argiment should point to a array for x, y, and x */
void MPU6500_GetData(int16_t *AccData, int16_t *GyroData)
{
    // grab the data from the MPU6500
    // readRegisters(ACCEL_OUT, 14, _buffer);
    readRegisters(0x3B, 1, &_buffer[0]);
    readRegisters(0x3C, 1, &_buffer[1]);
    readRegisters(0x3D, 1, &_buffer[2]);
    readRegisters(0x3E, 1, &_buffer[3]);
    readRegisters(0x3F, 1, &_buffer[4]);
    readRegisters(0x40, 1, &_buffer[5]);

    readRegisters(0x43, 1, &_buffer[6]);
    readRegisters(0x44, 1, &_buffer[7]);
    readRegisters(0x45, 1, &_buffer[8]);
    readRegisters(0x46, 1, &_buffer[9]);
    readRegisters(0x47, 1, &_buffer[10]);
    readRegisters(0x48, 1, &_buffer[11]);

    // combine into 16 bit values
    AccData[0] = (((int16_t)_buffer[0]) << 8) | _buffer[1];
    AccData[1] = (((int16_t)_buffer[2]) << 8) | _buffer[3];
    AccData[2] = (((int16_t)_buffer[4]) << 8) | _buffer[5];
    GyroData[0] = (((int16_t)_buffer[6]) << 8) | _buffer[7];
    GyroData[1] = (((int16_t)_buffer[8]) << 8) | _buffer[9];
    GyroData[2] = (((int16_t)_buffer[10]) << 8) | _buffer[11];
    double complementaryAngle[3];
    static float mpuOffset[3];
    for (int i = 0; i < 3; i++)
    {
        complementaryAngle[i] = 0;
        mpuOffset[i] = 0;
    }
    MPU6500_ComplementaryFilter(AccData, GyroData, complementaryAngle, mpuOffset);
    // BLE_LOG_I("MPU6500 1", "%.3f\n", (complementaryAngle[0]));
    // BLE_LOG_I("MPU6500 2", "%.3f\n", (complementaryAngle[1]));
    // BLE_LOG_I("MPU6500 3", "%.3f\n", (complementaryAngle[2]));
}