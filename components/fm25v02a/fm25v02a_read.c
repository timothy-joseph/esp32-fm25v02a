#include "include/fm25v02a.h"

#include <stdint.h>

#include <driver/spi_master.h>

#include "fm25v02a_spi_halfduplex_internal.h"
#include "fm25v02a_command_opcodes.h"

/**
 * This function is used to read a undetermined amount of data from the fram
 * chip
 *
 * @param dev The fram device
 * @param addr The address at which to start reading from
 * @param data A array of bytes which will contain the data red from the fram
 *	       chip. This array must be allocated (statically or dynamically)
 *	       by the user to be atleast of len size
 * @param len The amount of bytes to read from data
 *
 * @returns same as fram_spi_transmit_halfduplex
 */
esp_err_t
fram_read(fram_device_t *dev, uint16_t addr, uint8_t *data,
	  size_t len)
{
	uint8_t cmd[3] = {READ_CMD};

	cmd[0] = READ_CMD;
	cmd[1] = addr >> 8;
	cmd[2] = addr & 0xff;

	return fram_spi_transmit_halfduplex(dev, cmd, sizeof(cmd), data, len);
}

/**
 * This function is used to read a undetermined amount of data from the fram
 * chip. This function is implemented for the sake of completeness. Do not
 * use it. Despite its name being fastread, it's actually slower because it
 * inserts a dummy byte when transmitting the address.
 *
 * @param dev The fram device
 * @param addr The address at which to start reading from
 * @param data A array of bytes which will contain the data red from the fram
 *	       chip. This array must be allocated (statically or dynamically)
 *	       by the user to be atleast of len size
 * @param len The amount of bytes to read from data
 *
 * @returns same as fram_spi_transmit_halfduplex
 */
esp_err_t
fram_fast_read(fram_device_t *dev, uint16_t addr, uint8_t *data,
	       size_t len)
{
	uint8_t cmd[4] = {FSTRD_CMD};

	cmd[0] = FSTRD_CMD;
	cmd[1] = addr >> 8;
	cmd[2] = addr & 0xff;
	cmd[3] = 0;

	return fram_spi_transmit_halfduplex(dev, cmd, sizeof(cmd), data, len);
}
