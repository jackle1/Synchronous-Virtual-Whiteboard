#include <stdio.h>
#include "I2C_core.h"
#include "terasic_includes.h"
#include "mipi_camera_config.h"
#include "mipi_bridge_config.h"
#include "auto_focus.h"

int main(void)
{
	int hps_f = open("/dev/mem", (O_RDWR | O_SYNC));
	printf("HPS FD: %d\n", hps_f);
	lw_bridge_ptr = mmap(NULL, LW_BRIDGE_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, hps_f, LW_BRIDGE_BASE);
	volatile uint32_t *sdram = mmap(NULL, HPS_SDRAM_SPAN, (PROT_READ | PROT_WRITE), MAP_SHARED, hps_f, HPS_SDRAM_BASE);
	printf("DE1-SoC D8M VGA Demo\n");
	IOWR(MIPI_PWDN_N_BASE, 0x00, 0x00);
	IOWR(MIPI_RESET_N_BASE, 0x00, 0x00);

	sleep_us(2000);
	IOWR(MIPI_PWDN_N_BASE, 0x00, 0xFF);
	sleep_us(2000);
	IOWR(MIPI_RESET_N_BASE, 0x00, 0xFF);

	sleep_us(2000);

	// MIPI Init
	if (!MIPI_Init())
	{
		printf("MIPI_Init Init failed!\r\n");
	}
	else
	{
		printf("MIPI_Init Init successfully!\r\n");
	}

	mipi_clear_error();
	sleep_us(50 * 1000);
	mipi_clear_error();
	sleep_us(1000 * 1000);
	mipi_show_error_info();
	//	    mipi_show_error_info_more();
	printf("\n");

	uint16_t bin_level = DEFAULT_LEVEL;
	uint8_t manual_focus_step = 10;
	uint16_t current_focus = 300;
	// printf("Focusing\n");
	// Focus_Init();
	// printf("Done focusing?\n");

	IOWR(MIPI_PWDN_N_BASE, 0x00, 0x00);
	for (int i = 0; i < 640 * 480 * 3; i++)
	{
		sdram[i] = (uint32_t) -1;
	}
	printf("Starting loop\n");

	while (1)
	{
		// touch KEY0 to trigger Auto focus
		if ((IORD(KEY_BASE, 0) & 0x0F) == 0x0E)
		{
			// current_focus = Focus_Window(320,240);
			IOWR(MIPI_PWDN_N_BASE, 0, ~IORD(MIPI_PWDN_N_BASE, 0));
			volatile unsigned int *rgb = sdram;
			for (int i = 0; i < 640 * 480; i++)
			{
				rgb[i] = 0x6EFF7C;
			}
			printf("Done KEY0\n");
		}

		// touch KEY1 to trigger Manual focus  - step
		if ((IORD(KEY_BASE, 0) & 0x0F) == 0x0D)
		{

			if (current_focus > manual_focus_step)
				current_focus -= manual_focus_step;
			else
				current_focus = 0;
			OV8865_FOCUS_Move_to(current_focus);
		}

		// touch KEY2 to trigger Manual focus  + step
		if ((IORD(KEY_BASE, 0) & 0x0F) == 0x0B)
		{
			current_focus += manual_focus_step;
			if (current_focus > 1023)
				current_focus = 1023;
			OV8865_FOCUS_Move_to(current_focus);
		}

		// touch KEY3 to ZOOM
		if ((IORD(KEY_BASE, 0) & 0x0F) == 0x07)
		{
			if (bin_level == 3)
				bin_level = 1;
			else
				bin_level++;
			printf("set bin level to %d\n", bin_level);
			MIPI_BIN_LEVEL(bin_level);
			sleep_us(500000);
		}

		sleep_us(200000);
	};
	return 0;
}
