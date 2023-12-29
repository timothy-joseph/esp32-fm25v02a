#include "include/fm25v02a.h"

#include <stdint.h>

#include <driver/spi_master.h>

#include "fm25v02a_spi_halfduplex_internal.h"
#include "fm25v02a_command_opcodes.h"

/**
 * This function is used to read the device id of the fram chip. The device id
 * is 9 bytes
 *
 * @param dev The fram device
 * @param ret_id A array of 9 bytes that the user must allocate
 *
 * @returns Same as fram_spi_transmit_halfduplex
 */
esp_err_t
fram_read_device_id(fram_device_t *dev, uint8_t ret_id[9])
{
	uint8_t cmd = RDID_CMD;

	return fram_spi_transmit_halfduplex(dev, &cmd, sizeof(cmd), ret_id, 9);
}
