#include "include/fm25v02a.h"

#include "fm25v02a_spi_simple_cmd_internal.h"
#include "fm25v02a_command_opcodes.h"

/**
 * This function is used to send the command WREN which enables writing to the
 * fram chip. After each write operation (every type of write), writing is
 * disabled by default (hardware related).
 *
 * @param dev The fram device
 *
 * @returns Same as fram_spi_transmit_simple_cmd
 */
esp_err_t
fram_write_enable(fram_device_t *dev)
{
	return fram_spi_transmit_simple_cmd(dev, WREN_CMD);
}

/**
 * This function is used to send the command WRDI which disables writing to the
 * fram chip.
 *
 * @param dev The fram device
 *
 * @returns Same as fram_spi_transmit_simple_cmd
 */
esp_err_t
fram_write_disable(fram_device_t *dev)
{
	return fram_spi_transmit_simple_cmd(dev, WRDI_CMD);
}


