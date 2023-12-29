#include <driver/gpio.h>
#include <driver/spi_master.h>

#include "include/fm25v02a.h"

#define FREQ_MAX 40000000

static void set_cs(spi_transaction_t *t);
static void clear_cs(spi_transaction_t *t);

esp_err_t
fram_init(fram_device_t *ret, spi_host_device_t host, gpio_num_t cs,
	  uint32_t freq)
{
	/* TODO: probably not right */
	spi_device_interface_config_t spi_dev_cfg = {
		.command_bits = 8,
		.address_bits = 16,
		.dummy_bits = 0,
		.mode = 0,
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

esp_err_t
fram_write_enable(fram_device_t *dev)
{
}

esp_err_t
fram_write_disable(fram_device_t *dev)
{
}

esp_err_t
fram_read_status_register(fram_device_t *dev)
{
}

esp_err_t
fram_write_status_register(fram_device_t *dev, uint8_t force_enable)
{
}

esp_err_t
fram_read(fram_device_t *dev, uint16_t addr, uint8_t *data,
	  size_t len)
{
}

esp_err_t
fram_fast_read(fram_device_t *dev, uint16_t addr, uint8_t *data,
	       size_t len)
{
}

esp_err_t
fram_write(fram_device_t *dev, uint16_t addr, uint8_t *data, size_t len,
	   uint8_t force_enable)
{
}

esp_err_t
fram_sleep(fram_device_t *dev)
{
}

esp_err_t
fram_read_device_id(fram_device_t *dev)
{
}

static void
set_cs(spi_transaction_t *t)
{
}

static void
clear_cs(spi_transaction_t *t)
{
}
