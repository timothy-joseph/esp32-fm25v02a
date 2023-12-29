#ifndef FM25V02A_H
#define FM25V02A_H

#include <driver/gpio.h>
#include <driver/spi_master.h>

/*
 * macros used by the user and inside of the driver
 * example:
 *	if (fram_read_status_register(...) & STATUS_WEL_MASK)
 *		do_something();
 *
 * these indicate the bits used inside of the status register
 */
#define STATUS_WEL_MASK 0x02
#define STATUS_BLOCK_PROTECTION_MASK 0x0c
#define STATUS_WPEN_MASK 0x80

/*
 * macros used by the driver to determine to how many bits to shift the input
 * given by the user for the functions fram_set_status_bp, fram_set_status_wel,
 * and fram_set_status_wpen. and to determine by how many bits to shift the
 * output from the chip when the user calls the read-equivalent of those
 * functions
 *
 * the functions described here are TODO
 */
#define STATUS_WEL_OFFSET 0
#define STATUS_BLOCK_PROTECTION_OFFSET 2
#define STATUS_WPEN_OFFSET 7

/* TODO: add interrupt support */
typedef struct fram_device fram_device_t;
struct {
	spi_host_device_t host;
	gpio_num_t cs;
	uint8_t use_interrupt;
	spi_device_handle_t spi_dev;
} fram_device;

esp_err_t fram_init(fram_device_t *ret, spi_host_device_t host, gpio_num_t cs,
		    uint32_t freq);
esp_err_t fram_write_enable(fram_device_t *dev);
esp_err_t fram_write_disable(fram_device_t *dev);
esp_err_t fram_read_status_register(fram_device_t *dev);
esp_err_t fram_write_status_register(fram_device_t *dev, uint8_t force_enable);
esp_err_t fram_read(fram_device_t *dev, uint16_t addr, uint8_t *data,
		    size_t len);
esp_err_t fram_fast_read(fram_device_t *dev, uint16_t addr, uint8_t *data,
			 size_t len);
esp_err_t fram_write(fram_device_t *dev, uint16_t addr, uint8_t *data, size_t len,
		     uint8_t force_enable);
esp_err_t fram_sleep(fram_device_t *dev);
esp_err_t fram_read_device_id(fram_device_t *dev);

#endif /* #ifndef FM25V02A_H */
