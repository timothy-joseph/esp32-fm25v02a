#ifndef FM25V02A_H
#define FM25V02A_H

#include <stdint.h>

#include <driver/gpio.h>
#include <driver/spi_master.h>

/**
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
 */
#define STATUS_WEL_OFFSET 1
#define STATUS_BLOCK_PROTECTION_OFFSET 2
#define STATUS_WPEN_OFFSET 7

/* TODO: add interrupt support */
struct fram_device{
	spi_host_device_t host;
	gpio_num_t cs;
	uint8_t use_interrupt;
	spi_device_handle_t spi_dev;
};
typedef struct fram_device fram_device_t;

/*
 * Function documentation is inside of the .c source files
 */

/* Functions concerning the initialization and deinitialization of the
 * device: fm25v02a_init.c
 */
esp_err_t fram_init(fram_device_t *ret, spi_host_device_t host, gpio_num_t cs,
		    uint32_t freq);

/* Functions concerning the write enable/disable command:
 * fm25v02a_write_enable_disable.c
 */
esp_err_t fram_write_enable(fram_device_t *dev);
esp_err_t fram_write_disable(fram_device_t *dev);

/* Functions concerning the status register read command:
 * fm25v02a_read_status_register.c
 */
esp_err_t fram_read_status_register(fram_device_t *dev, uint8_t *ret);
esp_err_t fram_read_status_register_wel(fram_device_t *dev, uint8_t *ret);
esp_err_t fram_read_status_register_bp(fram_device_t *dev, uint8_t *ret);
esp_err_t fram_read_status_register_wpen(fram_device_t *dev, uint8_t *ret);

/* Functions concerning the status register write command
 * fm25v02a_write_status_register.c
 */
esp_err_t fram_write_status_register(fram_device_t *dev, uint8_t data,
				     uint8_t force_enable);
esp_err_t fram_write_status_register_wel(fram_device_t *dev, uint8_t data,
					 uint8_t force_enable);
esp_err_t fram_write_status_register_bp(fram_device_t *dev, uint8_t data,
					uint8_t force_enable);

/* Functions concerning the read commands
 * fm25v02a_read.c
 */
esp_err_t fram_read(fram_device_t *dev, uint16_t addr, uint8_t *data,
		    size_t len);
esp_err_t fram_fast_read(fram_device_t *dev, uint16_t addr, uint8_t *data,
			 size_t len);

/* Functions concerning the write commands 
 * fm25v02a_write.c
 */
esp_err_t fram_write(fram_device_t *dev, uint16_t addr, uint8_t *data, size_t len,
		     uint8_t force_enable);

/* Functions concerning the sleep mode
 * fm25v02a_sleep.c
 */
esp_err_t fram_sleep(fram_device_t *dev);
void fram_wakeup(fram_device_t *dev);

/* Functions concerning the device id
 * fm25v02a_device_id.c
 */
esp_err_t fram_read_device_id(fram_device_t *dev, uint8_t ret_id[9]);

#endif /* #ifndef FM25V02A_H */
