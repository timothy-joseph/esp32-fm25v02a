#include <driver/gpio.h>
#include <driver/spi_master.h>

#include "include/fm25v02a.h"

#define FREQ_MAX 40000000

#define WREN_CMD 0x06
#define WRDI_CMD 0x04
#define RDSR_CMD 0x05
#define WRSR_CMD 0x01
#define READ_CMD 0x03
#define FSTRD_CMD 0x0b
#define WRITE_CMD 0x02
#define SLEEP_CMD 0xb9
#define RDID_CMD 0x9f

static void set_cs(spi_transaction_t *t);
static void clear_cs(spi_transaction_t *t);

/**
 * Function used to initialize the device pointed to by ret
 *
 * @param ret The device to initialize
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
	return fram_spi_transmit_simple_cmd(dev, WREN_CMD);
}

esp_err_t
fram_write_disable(fram_device_t *dev)
{
	return fram_spi_transmit_simple_cmd(dev, WRDI_CMD);
}

esp_err_t
fram_read_status_register(fram_device_t *dev, uint8_t *ret)
{
	uint8_t cmd = RDSR_CMD;

	if (ret == NULL)
		return ESP_ERR_INVALID_ARG;

	return fram_spi_transmit_halfduplex(dev, &cmd, sizeof(cmd), ret,
					    sizeof(*ret));
}

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

esp_err_t
fram_read(fram_device_t *dev, uint16_t addr, uint8_t *data,
	  size_t len)
{
	uint8_t cmd[3] = {READ_CMD}

	cmd[0] = READ_CMD;
	cmd[1] = addr >> 8;
	cmd[2] = addr & 0xff;

	return fram_spi_transmit_halfduplex(dev, cmd, sizeof(cmd), data, len);
}

esp_err_t
fram_fast_read(fram_device_t *dev, uint16_t addr, uint8_t *data,
	       size_t len)
{
	uint8_t cmd[4] = {FSTRD_CMD}

	cmd[0] = FSTRD_CMD;
	cmd[1] = addr >> 8;
	cmd[2] = addr & 0xff;
	cmd[3] = 0;

	return fram_spi_transmit_halfduplex(dev, cmd, sizeof(cmd), data, len);
}

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
	if (force_enable)
		fram_write_enable(dev);
}

esp_err_t
fram_sleep(fram_device_t *dev)
{
	return fram_spi_transmit_simple_cmd(dev, SLEEP_CMD);
}

esp_err_t
fram_read_device_id(fram_device_t *dev)
{
	return fram_spi_transmit_simple_cmd(dev, RDID_CMD);
}

static void
set_cs(spi_transaction_t *t)
{
}

static void
clear_cs(spi_transaction_t *t)
{
}
