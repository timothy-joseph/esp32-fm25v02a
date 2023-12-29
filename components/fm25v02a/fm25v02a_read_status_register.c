#include "include/fm25v02a.h"

#include <stdint.h>

#include <driver/spi_master.h>

#include "fm25v02a_spi_halfduplex_internal.h"
#include "fm25v02a_command_opcodes.h"

static esp_err_t fram_read_status_reg_bits(fram_device_t *dev, uint8_t *ret,
					   uint8_t mask, uint8_t offset);


/**
 * This function is used to read the status register of the fram chip.
 *
 * @param dev The fram device
 * @param ret A pointer to a single byte in which the result of the read
 *	      operation will be put
 *
 * @returns
 *	- ESP_ERR_INVALID_ARG if ret is NULL
 *	- same as fram_spi_transmit_halfduplex otherwise
 */
esp_err_t
fram_read_status_register(fram_device_t *dev, uint8_t *ret)
{
	uint8_t cmd = RDSR_CMD;

	if (ret == NULL)
		return ESP_ERR_INVALID_ARG;

	return fram_spi_transmit_halfduplex(dev, &cmd, sizeof(cmd), ret,
					    sizeof(*ret));
}

/**
 * This function is used to get the value of the WEL bit inside of the status
 * register
 *
 * @param dev The fram device
 * @param ret A pointer to a single byte in which the value of the WEL bit
 *	      will be put aligned starting with bit 0, so *ret = 0 if WEL = 0
 *	      and *ret = 1 if WEL = 1
 *
 * @returns same as fram_read_status_reg_bits
 */
esp_err_t
fram_read_status_register_wel(fram_device_t *dev, uint8_t *ret)
{
	return fram_read_status_reg_bits(dev, ret, STATUS_WEL_MASK,
					 STATUS_WEL_OFFSET);
}

/**
 * This function is used to get the value of the BP bits inside of the status
 * register
 *
 * @param dev The fram device
 * @param ret A pointer to a single byte in which the value of the BP bits
 *	      will be put aligned starting with bits 0, so *ret = 0 if BP = 0
 *	      and *ret = 1 if BP = 1
 *
 * @returns same as fram_read_status_reg_bits
 */
esp_err_t
fram_read_status_register_bp(fram_device_t *dev, uint8_t *ret)
{
	return fram_read_status_reg_bits(dev, ret, STATUS_BLOCK_PROTECTION_MASK,
					 STATUS_BLOCK_PROTECTION_OFFSET);
}

/**
 * This function is used to get the value of the WEL bit inside of the status
 * register
 *
 * @param dev The fram device
 * @param ret A pointer to a single byte in which the value of the WEL bit
 *	      will be put aligned starting with bit 0, so *ret = 0 if WEL = 0
 *	      and *ret = 1 if WEL = 1
 *
 * @returns same as fram_read_status_reg_bits
 */
esp_err_t
fram_read_status_register_wpen(fram_device_t *dev, uint8_t *ret)
{
	return fram_read_status_reg_bits(dev, ret, STATUS_WPEN_MASK,
					 STATUS_WPEN_OFFSET);
}

/**
 * This is a internal function used to read the status register, apply a mask
 * to the result and then shift to the right by offset bits
 *
 * @param dev The fram device
 * @param ret A pointer to a byte in which the result of this function will be
 *	      stored in the case of normal operation and 0 if the function
 *	      failed
 * @param mask The mask to apply to the value read from the status register
 * @param offset The number of bits to shift to the right the masked result
 *
 * returns
 *	- ESP_ERR_INVALID_ARG if ret == NULL
 *	- same as fram_read_status_register otherwise
 */
static esp_err_t
fram_read_status_reg_bits(fram_device_t *dev, uint8_t *ret, uint8_t mask,
			  uint8_t offset)
{
	esp_err_t err;
	uint8_t sr;

	if (ret == NULL)
		return ESP_ERR_INVALID_ARG;
	
	err = fram_read_status_register(dev, &sr);
	if (err != ESP_OK) {
		*ret = 0;
		return err;
	}

	*ret = (sr & mask) >> offset;

	return err;
}
