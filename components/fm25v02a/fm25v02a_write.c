#include "include/fm25v02a.h"

#include <stdint.h>

#include <driver/spi_master.h>
#include "fm25v02a_command_opcodes.h"

/**
 * This function is used to write a data to the fram chip.
 *
 * @param dev The fram device
 * @param addr The address at which to start writing to
 * @param data A array of bytes which will be writtinen into the fram starting
 *	       at addr
 * @param len The amount of bytes to write from data
 * @param force_enable If force_enable is set, then this function will first
 *		       call fram_write_enable and then transmit the data.
 *		       If force_enable is not set, then the function will go
 *		       directly to transmitting the data, even if writing is
 *		       disabled
 *
 * @returns
 *	- ESP_ERR_INVALID_ARG if dev == NULL
 *	- same as fram_spi_transmit_halfduplex otherwise
 */
esp_err_t
fram_write(fram_device_t *dev, uint16_t addr, uint8_t *data, size_t len,
	   uint8_t force_enable)
{
	/* with the other commands i was able to use a universal function
	 * that i created called fram_spi_transmit_halfduplex because
	 * i didn't need to write a undetermined amount of data, just
	 * receive a undetermined amount of data
	 */
	/* TODO: acquire spi */
	spi_transaction_ext_t ext_t = {0};

	if (dev == NULL)
		return ESP_ERR_INVALID_ARG;

	/*
	 * previously in the device configuration, the command_bits,
	 * address_bits, and dummy bits were set to 0
	 */
	ext_t.base.flags = SPI_TRANS_VARIABLE_CMD | SPI_TRANS_VARIABLE_ADDR;
	ext_t.base.cmd = WRITE_CMD;
	ext_t.base.addr = addr;
	ext_t.base.tx_buffer = data;
	ext_t.base.length = len * 8;
	ext_t.base.user = dev;
	ext_t.base.rx_buffer = NULL;
	ext_t.base.rxlength = 0;
	ext_t.command_bits = 8;
	ext_t.address_bits = 16;
	ext_t.dummy_bits = 0;

	if (force_enable)
		fram_write_enable(dev);
	
	return spi_device_polling_transmit(dev->spi_dev, &ext_t.base); 
}
