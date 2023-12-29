#include "include/fm25v02a.h"

#include <stdint.h>

#include <driver/spi_master.h>
#include <esp_timer.h>

#include "fm25v02a_spi_simple_cmd_internal.h"
#include "fm25v02a_command_opcodes.h"

/**
 * This function is used to send the command SLEEP which puts the device in
 * sleep mode
 *
 * @param dev The fram device
 *
 * @returns Same as fram_spi_transmit_simple_cmd
 */
esp_err_t
fram_sleep(fram_device_t *dev)
{
	return fram_spi_transmit_simple_cmd(dev, SLEEP_CMD);
}

/**
 * This function is used to wakeup the device
 *
 * The datasheet recomends sending a dummy command then wait the remaining
 * time from Trec, which is 400 us
 *
 * @param dev The fram device
 */
void
fram_wakeup(fram_device_t *dev)
{
	int64_t us;

	us = esp_timer_get_time();
	if (fram_spi_transmit_simple_cmd(dev, RDSR_CMD) == ESP_OK)
		while (esp_timer_get_time() - us < 400);
}
