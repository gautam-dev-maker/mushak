#include "mpu6500.h"
#include "log.h"

#define TAG "MPU6500"

static SPI_HandleTypeDef hspi1;

// TODO: Remove this buffer, instead return
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
void mpu6500_write_register(uint8_t subAddress, uint8_t data)
{
    MPU_SPI_Write(&data, subAddress, 1);
    HAL_Delay(10);
}

/* reads registers from MPU6500 given a starting register address, number of bytes, and a pointer to store data */
void mpu6500_read_registers(uint8_t subAddress, uint8_t count, uint8_t *dest)
{
    MPU_SPI_Read(dest, subAddress, count);
}

/* gets the MPU6500 WHO_AM_I register value, expected to be 0x71 */
static uint8_t whoAmI()
{
    uint8_t data;
    // read the WHO AM I register
    mpu6500_read_registers(WHO_AM_I, 1, &data);

    // return the register value
    return data;
}

/* starts communication with the MP6500 */
HAL_StatusTypeDef mpu6500_init()
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
    HAL_StatusTypeDef err = HAL_SPI_Init(&hspi1);
    HAL_ERROR_CHECK(err == HAL_OK, "Failed to initialise the SPI Driver", err);

    // select clock source to gyro
    mpu6500_write_register(PWR_MGMNT_1, CLOCK_SEL_PLL);
    // enable I2C master mode
    mpu6500_write_register(USER_CTRL, I2C_MST_EN);
    // set the I2C bus speed to 400 kHz
    mpu6500_write_register(I2C_MST_CTRL, I2C_MST_CLK);

    // reset the MPU6500
    mpu6500_write_register(PWR_MGMNT_1, PWR_RESET);
    // wait for MPU-6500 to come back up
    HAL_Delay(10);
    
    // select clock source to gyro
    mpu6500_write_register(PWR_MGMNT_1, CLOCK_SEL_PLL);

    // check the WHO AM I byte, expected value is 0x71 (decimal 113) or 0x73 (decimal 115)
    uint8_t who = whoAmI();
    HAL_ERROR_CHECK(who == 0x75, "Unable to verify the address of whoami register", HAL_ERROR);

    // enable accelerometer and gyro
    mpu6500_write_register(PWR_MGMNT_2, SEN_ENABLE);

    // setting accel range to 16G as default
    mpu6500_write_register(ACCEL_CONFIG, ACCEL_FS_SEL_16G);

    // setting the gyro range to 2000DPS as default
    mpu6500_write_register(GYRO_CONFIG, GYRO_FS_SEL_250DPS);

    // setting bandwidth to 184Hz as default
    mpu6500_write_register(ACCEL_CONFIG2, DLPF_184);

    // setting gyro bandwidth to 184Hz
    mpu6500_write_register(CONFIG, DLPF_184);

    // setting the sample rate divider to 0 as default
    mpu6500_write_register(SMPDIV, 0x00);

    // enable I2C master mode
    mpu6500_write_register(USER_CTRL, I2C_MST_EN);

    // set the I2C bus speed to 400 kHz
    mpu6500_write_register(I2C_MST_CTRL, I2C_MST_CLK);

    // select clock source to gyro
    mpu6500_write_register(PWR_MGMNT_1, CLOCK_SEL_PLL);

    // successful init, return 0
    return HAL_OK;
}

/* sets the accelerometer full scale range to values other than default */
void mpu6500_set_accel_range(AccelRange range)
{
    mpu6500_write_register(ACCEL_CONFIG, range);
}

/* sets the gyro full scale range to values other than default */
void mpu6500_get_gyro_range(GyroRange range)
{
    mpu6500_write_register(GYRO_CONFIG, range);
}

/* sets the DLPF bandwidth to values other than default */
void mpu6500_set_dlpf_bandwidth(DLPFBandwidth bandwidth)
{
    mpu6500_write_register(ACCEL_CONFIG2, bandwidth);
    mpu6500_write_register(CONFIG, bandwidth);
}
/* read the data, each argiment should point to a array for x, y, and x */
void mpu6500_get_data(int16_t *AccData, int16_t *GyroData)
{
    // grab the data from the MPU6500
    mpu6500_read_registers(ACCEL_OUT, 14, _buffer);

    // combine into 16 bit values
    AccData[0] = (((int16_t)_buffer[0]) << 8) | _buffer[1];
    AccData[1] = (((int16_t)_buffer[2]) << 8) | _buffer[3];
    AccData[2] = (((int16_t)_buffer[4]) << 8) | _buffer[5];
    GyroData[0] = (((int16_t)_buffer[8]) << 8) | _buffer[9];
    GyroData[1] = (((int16_t)_buffer[10]) << 8) | _buffer[11];
    GyroData[2] = (((int16_t)_buffer[12]) << 8) | _buffer[13];
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(spiHandle->Instance==SPI1)
    {
        /* SPI1 clock enable */
        __HAL_RCC_SPI1_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**SPI1 GPIO Configuration
            PB3     ------> SPI1_SCK
            PB4     ------> SPI1_MISO
            PB5     ------> SPI1_MOSI
        */
        GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{

    if(spiHandle->Instance==SPI1)
    {
        /* Peripheral clock disable */
        __HAL_RCC_SPI1_CLK_DISABLE();

        /**SPI1 GPIO Configuration
            PB3     ------> SPI1_SCK
            PB4     ------> SPI1_MISO
            PB5     ------> SPI1_MOSI
        */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);
    }
}
