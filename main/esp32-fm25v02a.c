#include <stdio.h>

#include <driver/spi_master.h>

#include <fm25v02a.h>

#define CS 15
#define SCLK 14
#define MOSI 13
#define MISO 12

void app_main(void)
{
	fram_device_t dev;
	spi_bus_config_t spi_cfg = {
		.mosi_io_num = MOSI,
		.miso_io_num = MISO,
		.sclk_io_num = SCLK,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.data4_io_num = -1,
		.data5_io_num = -1,
		.data6_io_num = -1,
		.data7_io_num = -1,
		.max_transfer_sz = 0,
		.flags = SPICOMMON_BUSFLAG_MASTER,
		/* TODO */
		.isr_cpu_id = 0,
		.intr_flags = 0
	};
	size_t i;
	uint8_t data;
	uint8_t to_write[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	uint8_t read_from_fram[10] = {0};

	spi_bus_initialize(SPI2_HOST, &spi_cfg, SPI_DMA_DISABLED);

	fram_init(&dev, SPI2_HOST, CS, 40000000);

	fram_write_status_register(&dev, 0, 1);
	fram_read_status_register(&dev, &data);
	/* should output 0 */
	printf("%hhx\n", data);

	fram_write_status_register_wel(&dev, 1, 1);
	fram_write_status_register_bp(&dev, 3, 1);
	fram_read_status_register(&dev, &data);
	/* should output 0e or 8e */
	printf("%hhx\n", data);

	fram_write_status_register(&dev, 0, 1);
	fram_read_status_register(&dev, &data);
	/* should output 0 */
	printf("%hhx\n", data);

	/* should output 1 2 3 4 5 6 7 9 a */
	fram_write(&dev, 123, to_write, sizeof(to_write), 1);
	fram_read(&dev, 123, read_from_fram, sizeof(read_from_fram));

	for (i = 0; i < sizeof(read_from_fram); i++)
		printf("%x ", read_from_fram[i]);
	printf("\n");

	while (1);
}
