#include "include/fm25v02a.h"

#include <stdint.h>

#include <driver/spi_master.h>

#include "fm25v02a_spi_halfduplex_internal.h"
#include "fm25v02a_command_opcodes.h"

static esp_err_t fram_write_status_register_bits(fram_device_t *dev,
						 uint8_t data,
						 uint8_t force_enable,
						 uint8_t mask, uint8_t offset);

/**
 * This function is used to write to the status register of the fram chip.
 *
 * @param dev The fram device
 * @param data The byte to write to the status register
 * @param force_enable If force_enable is set, then this function will first
 *		       call fram_write_enable and then transmit the data.
 *		       If force_enable is not set, then the function will go
 *		       directly to transmitting the data, even if writing is
 *		       disabled
 *
 * @returns same as fram_spi_transmit_halfduplex
 */
esp_err_t
fram_write_status_register(fram_device_t *dev, uint8_t data,
			   uint8_t force_enable)
{
	uint8_t cmd[2] = {WRSR_CMD};

	/* TODO: acquire spi */

	cmd[0] = WRSR_CMD;
	cmd[1] = data;
	if (force_enable)
		fram_write_enable(dev);
	
	return fram_spi_transmit_halfduplex(dev, cmd, sizeof(cmd), NULL, 0);
}

/**
 * This function is used to write the WEL bit inside of the status register
 * This is done by first reading the status register, modifying the bits
 * locally and then writing the modified byte to the status register.
 *
 * @param dev The fram device
 * @param data The WEL bits to write to the status register given with the LSB
 *	       at bit 0
 * @param force_enable After modifying the red status register:
 *		       If force_enable is set, then this function will first
 *		       call fram_write_enable and then write the new status
 *		       register data.
 *		       If force_enable is not set, then the function will go
 *		       directly to writing the new status register
 *
 * @returns same as fram_write_status_register_bits
 */
esp_err_t
fram_write_status_register_wel(fram_device_t *dev, uint8_t data,
			       uint8_t force_enable)
{
	return fram_write_status_register_bits(dev, data, force_enable,
					       STATUS_WEL_MASK,
					       STATUS_WEL_OFFSET);
}

/**
 * This function is used to write the BP bit inside of the status register
 * This is done by first reading the status register, modifying the bits
 * locally and then writing the modified byte to the status register.
 *
 * @param dev The fram device
 * @param data The BP bits to write to the status register given with the LSB at
 *	       bit 0
 * @param force_enable After modifying the red status register:
 *		       If force_enable is set, then this function will first
 *		       call fram_write_enable and then write the new status
 *		       register data.
 *		       If force_enable is not set, then the function will go
 *		       directly to writing the new status register
 *
 * @returns same as fram_write_status_register_bits
 */
esp_err_t
fram_write_status_register_bp(fram_device_t *dev, uint8_t data,
			      uint8_t force_enable)
{
	return fram_write_status_register_bits(dev, data, force_enable,
					       STATUS_BLOCK_PROTECTION_MASK,
					       STATUS_BLOCK_PROTECTION_OFFSET);
}

/**
 * This is a internal function used to read the status register, modify the
 * red data by setting the bits pointed to by mask and offset to the parameter
 * data and then writing the result back into the status register
 *
 * @param dev The fram device
 * @param data The bits to write to the status register given with the LSB at
 *	       bit 0
 * @param force_enable After modifying the red status register:
 *		       If force_enable is set, then this function will first
 *		       call fram_write_enable and then write the new status
 *		       register data.
 *		       If force_enable is not set, then the function will go
 *		       directly to writing the new status register
 *
 * @returns
 *	- what fram_read_status_register returned if it was different from
 *	  ESP_OK
 *	- same as fram_write_status_register otherwise
 */
static esp_err_t
fram_write_status_register_bits(fram_device_t *dev, uint8_t data,
				uint8_t force_enable, uint8_t mask,
				uint8_t offset)
{
	esp_err_t err;
	uint8_t new_sr;

	err = fram_read_status_register(dev, &new_sr);
	if (err != ESP_OK)
		return err;
	
	new_sr &= ~mask;
	new_sr |= (data << offset) & mask;

	return fram_write_status_register(dev, new_sr, force_enable);
}
