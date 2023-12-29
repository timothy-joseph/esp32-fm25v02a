#ifndef FM25V02A_SPI_HALFDUPLEX_INTERNALS_H
#define FM25V02A_SPI_HALFDUPLEX_INTERNALS_H

#include "include/fm25v02a.h"

#include <stdint.h>

#include <driver/spi_master.h>

static esp_err_t fram_spi_transmit_halfduplex(fram_device_t *dev, uint8_t *tx,
					      size_t tx_size, uint8_t *rx,
					      size_t rx_size);

/**
 * This is a internal function used to perform a halfduplex transmission with
 * the send data as tx and the data to receive stored into rx.
 * The opcode and address (if needed) should be part of tx
 *
 * @param dev The fram device
 * @param tx The data to send
 * @param tx_size The number of bytes to send from tx
 * @param rx The receive buffer into which data will be put in the read part of
 *	     the transmission. Must be allocated by the user
 * @param rx_size The number of bytes to read
 *
 * @returns Same as fram_spi_transmit_halfduplex
 */
static esp_err_t
fram_spi_transmit_halfduplex(fram_device_t *dev, uint8_t *tx, size_t tx_size,
			     uint8_t *rx, size_t rx_size)
{
	/* TODO: use_interrupt support */
	spi_transaction_t t = {0};

	/*
	 * previously in the device configuration, the command_bits,
	 * address_bits, and dummy bits were set to 0
	 */
	t.tx_buffer = tx;
	t.length = tx_size * 8;
	t.rx_buffer = rx;
	t.rxlength = rx_size * 8;
	t.user = dev;

	return spi_device_polling_transmit(dev->spi_dev, &t);
}

#endif /* FM25V02A_SPI_HALFDUPLEX_INTERNALS_H */
