#include "include/fm25v02a.h"

#include <stdint.h>

#include <driver/gpio.h>
#include <driver/spi_master.h>

#define FREQ_MAX 40000000

static void set_cs(spi_transaction_t *t);
static void clear_cs(spi_transaction_t *t);

/**
 * Function used to initialize the device pointed to by ret
 *
 * @param ret After this call *ret will contain the context of the device.
 *	      This is the parameter you need to send to the other commands.
 * @param host The spi host (SPI1_HOST is not a valid parameter)
 * @param cs The gpio pin used for chip select
 * @param freq The desired frequency to run the device at (max freq is 40mhz)
 *
 * @return
 *  - ESP_OK: on success
 *  - ESP_ERR_INVALID_ARG: if any of the parameters passed are invalid
 */
esp_err_t
fram_init(fram_device_t *ret, spi_host_device_t host, gpio_num_t cs,
	  uint32_t freq)
{
	/* TODO: probably not right */
	spi_device_interface_config_t spi_dev_cfg = {
		.command_bits = 0,
		.address_bits = 0,
		.dummy_bits = 0,
		.mode = 0,
		.flags = SPI_DEVICE_HALFDUPLEX,
		.clock_source = SPI_CLK_SRC_DEFAULT,
		.clock_speed_hz = freq,
		.spics_io_num = -1,
		.pre_cb = clear_cs,
		.post_cb = set_cs
	};
	esp_err_t err;

	if (ret == NULL || host == SPI1_HOST || freq > FREQ_MAX)
		return ESP_ERR_INVALID_ARG;
	
	ret->host = host;
	ret->cs = cs;
	ret->use_interrupt = 0;

	err = spi_bus_add_device(host, &spi_dev_cfg, &ret->spi_dev);
	if (err != ESP_OK)
		return err;
	
	gpio_set_level(cs, 1);
	gpio_set_direction(cs, GPIO_MODE_OUTPUT);

	return ESP_OK;
}

/**
 * This is a internal function used inside of the device config to
 * set the cs pin high
 */
static void
set_cs(spi_transaction_t *t)
{
	/* t->user is fram_device_t * */
	gpio_set_level(((fram_device_t *)t->user)->cs, 1);
}

/**
 * This is a internal function used inside of the device config to
 * set the cs pin low
 */
static void
clear_cs(spi_transaction_t *t)
{
	gpio_set_level(((fram_device_t *)t->user)->cs, 0);
}
