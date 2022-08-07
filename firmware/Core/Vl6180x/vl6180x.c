#include "vl6180x.h"

static I2C_HandleTypeDef hi2c1;
// static i2c_dev_t dev;

HAL_StatusTypeDef vl6180x_init(I2C_TypeDef *instance, i2c_dev_t *dev)
{
	// I2C_HandleTypeDef hi2c1;
	hi2c1.Instance = instance;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	dev->i2c_handle = hi2c1;
	dev->dev_addr = 0x52;
	return i2c_init(dev);
}

HAL_StatusTypeDef vl6180x_configure(i2c_dev_t *dev)
{
	uint8_t val;
	i2c_read_8_bit(dev, 0x16, &val);
	while (val != 0x01)
	{
	}

	/* Mandatory: Private registers. */
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

	/* Recommended : Public registers - See data sheet for more detail */

	/* Enables polling for New Sample ready when measurement completes */
	i2c_write_8_bit(dev, 0x0011, 0x10);
	/* Set the averaging sample period (compromise between lower noise and
	 * increased execution time) */
	i2c_write_8_bit(dev, 0x010a, 0x30);
	/* Sets the light and dark gain (upper nibble). Dark gain should not be
	 * changed.*/
	i2c_write_8_bit(dev, 0x003f, 0x46);
	/* sets the # of range measurements after which auto calibration of system
	 * is performed */
	i2c_write_8_bit(dev, 0x0031, 0xFF);
	/* Set ALS integration time to 100ms */
	i2c_write_8_bit(dev, 0x0040, 0x63);
	/* perform a single temperature calibration of the ranging sensor */
	return i2c_write_8_bit(dev, 0x002e, 0x01);
}

HAL_StatusTypeDef vl6180x_measure_distance(i2c_dev_t *dev, uint8_t *out_mm)
{
	uint8_t status, mm;

	/* Wait for device ready. */
	do
	{
		i2c_read_8_bit(dev, VL6180X_RESULT_RANGE_STATUS, &status);
	} while ((status & (1 << 0)) == 0);

	/* Start measurement. */
	i2c_write_8_bit(dev, VL6180X_SYSRANGE_START, 0x01);

	/* Wait for measurement ready. */
	// do {
	//     status = i2c_read_8_bit(VL6180X_RESULT_INTERRUPT_STATUS_GPIO);
	// } while ((status & (1 << 2)) == 0);

	/* Read result. */
	i2c_read_8_bit(dev, VL6180X_RESULT_RANGE_VAL, &mm);
	*out_mm = mm;

	/* Clear interrupt flags. */
	i2c_write_8_bit(dev, VL6180X_SYSTEM_INTERRUPT_CLEAR, 0x07);

	/* Wait for device ready. */
	do
	{
		i2c_read_8_bit(dev, VL6180X_RESULT_RANGE_STATUS, &status);
	} while ((status & (1 << 0)) == 0);

	return HAL_OK;
}