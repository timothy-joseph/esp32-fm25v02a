#ifndef FM25V02A_SPI_SIMPLE_CMD_INTERNAL_H
#define FM25V02A_SPI_SIMPLE_CMD_INTERNAL_H

#include <stdint.h>

#include <fm25v02a.h>

#include "fm25v02a_spi_halfduplex_internal.h"

static esp_err_t fram_spi_transmit_simple_cmd(fram_device_t *dev, uint8_t cmd);

/**
 * This is a internal function used to send a simple command to the fram chip.
 * A simple command is a transmission that is exactly 1 byte
 *
 * @param dev The fram device
 * @param cmd The command to send
 *
 * @returns Same as fram_spi_transmit_halfduplex
 */
static esp_err_t
fram_spi_transmit_simple_cmd(fram_device_t *dev, uint8_t cmd)
{
	return fram_spi_transmit_halfduplex(dev, &cmd, sizeof(cmd), NULL, 0);
}


#endif /* #ifndef FM25V02A_SPI_SIMPLE_CMD_INTERNAL_H */
