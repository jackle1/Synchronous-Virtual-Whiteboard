// https://people.ece.cornell.edu/land/courses/ece5760/DE1_SOC/HPS_FPGA/pio_test/ghrd_top.v
module hps_system (
      ///////// CLOCK /////////
      input              CLOCK_50,

      ///////// DRAM /////////
      output      [12:0] DRAM_ADDR,
      output      [1:0]  DRAM_BA,
      output             DRAM_CAS_N,
      output             DRAM_CKE,
      output             DRAM_CLK,
      output             DRAM_CS_N,
      inout       [15:0] DRAM_DQ,
      output             DRAM_LDQM,
      output             DRAM_RAS_N,
      output             DRAM_UDQM,
      output             DRAM_WE_N,

      ///////// FAN /////////
      output             FAN_CTRL,

      ///////// FPGA /////////
      output             FPGA_I2C_SCLK,
      inout              FPGA_I2C_SDAT,

      ///////// GPIO /////////
      inout     [35:0]         GPIO_0,
      inout     [35:0]         GPIO_1,
 

      ///////// HEX0 /////////
      output      [6:0]  HEX0,

      ///////// HEX1 /////////
      output      [6:0]  HEX1,

      ///////// HEX2 /////////
      output      [6:0]  HEX2,

      ///////// HEX3 /////////
      output      [6:0]  HEX3,

      ///////// HEX4 /////////
      output      [6:0]  HEX4,

      ///////// HEX5 /////////
      output      [6:0]  HEX5,

      inout unsigned [1:0] HPS_GPIO,
      inout              HPS_CONV_USB_N,
      output      [14:0] HPS_DDR3_ADDR,
      output      [2:0]  HPS_DDR3_BA,
      output             HPS_DDR3_CAS_N,
      output             HPS_DDR3_CKE,
      output             HPS_DDR3_CK_N,
      output             HPS_DDR3_CK_P,
      output             HPS_DDR3_CS_N,
      output      [3:0]  HPS_DDR3_DM,
      inout       [31:0] HPS_DDR3_DQ,
      inout       [3:0]  HPS_DDR3_DQS_N,
      inout       [3:0]  HPS_DDR3_DQS_P,
      output             HPS_DDR3_ODT,
      output             HPS_DDR3_RAS_N,
      output             HPS_DDR3_RESET_N,
      input              HPS_DDR3_RZQ,
      output             HPS_DDR3_WE_N,
      output             HPS_ENET_GTX_CLK,
      inout              HPS_ENET_INT_N,
      output             HPS_ENET_MDC,
      inout              HPS_ENET_MDIO,
      input              HPS_ENET_RX_CLK,
      input       [3:0]  HPS_ENET_RX_DATA,
      input              HPS_ENET_RX_DV,
      output      [3:0]  HPS_ENET_TX_DATA,
      output             HPS_ENET_TX_EN,
      inout       [3:0]  HPS_FLASH_DATA,
      output             HPS_FLASH_DCLK,
      output             HPS_FLASH_NCSO,
      inout              HPS_GSENSOR_INT,
      inout              HPS_I2C1_SCLK,
      inout              HPS_I2C1_SDAT,
      inout              HPS_I2C2_SCLK,
      inout              HPS_I2C2_SDAT,
      inout              HPS_I2C_CONTROL,
      inout              HPS_KEY,
      inout              HPS_LED,
      inout              HPS_LTC_GPIO,
      output             HPS_SD_CLK,
      inout              HPS_SD_CMD,
      inout       [3:0]  HPS_SD_DATA,
      output             HPS_SPIM_CLK,
      input              HPS_SPIM_MISO,
      output             HPS_SPIM_MOSI,
      inout              HPS_SPIM_SS,
      input              HPS_UART_RX,
      output             HPS_UART_TX,
      input              HPS_USB_CLKOUT,
      inout       [7:0]  HPS_USB_DATA,
      input              HPS_USB_DIR,
      input              HPS_USB_NXT,
      output             HPS_USB_STP,

      ///////// KEY /////////
      input       [3:0]  KEY,

      ///////// LEDR /////////
      output      [9:0]  LEDR,

      ///////// SW /////////
      input       [9:0]  SW,

      ///////// TD /////////
      input              TD_CLK27,
      input      [7:0]  TD_DATA,
      input             TD_HS,
      output             TD_RESET_N,
      input             TD_VS,

      ///////// VGA /////////
      output      [7:0]  VGA_B,
      output             VGA_BLANK_N,
      output             VGA_CLK,
      output      [7:0]  VGA_G,
      output             VGA_HS,
      output      [7:0]  VGA_R,
      output             VGA_SYNC_N,
      output             VGA_VS
);

    cpu c (
        .clk_clk(CLOCK_50),                         //    clk.clk
          
        // Ethernet
        .hps_io_hps_io_gpio_inst_GPIO35	(HPS_ENET_INT_N),
        .hps_io_hps_io_emac1_inst_TX_CLK	(HPS_ENET_GTX_CLK),
        .hps_io_hps_io_emac1_inst_TXD0	(HPS_ENET_TX_DATA[0]),
        .hps_io_hps_io_emac1_inst_TXD1	(HPS_ENET_TX_DATA[1]),
        .hps_io_hps_io_emac1_inst_TXD2	(HPS_ENET_TX_DATA[2]),
        .hps_io_hps_io_emac1_inst_TXD3	(HPS_ENET_TX_DATA[3]),
        .hps_io_hps_io_emac1_inst_RXD0	(HPS_ENET_RX_DATA[0]),
        .hps_io_hps_io_emac1_inst_MDIO	(HPS_ENET_MDIO),
        .hps_io_hps_io_emac1_inst_MDC		(HPS_ENET_MDC),
        .hps_io_hps_io_emac1_inst_RX_CTL	(HPS_ENET_RX_DV),
        .hps_io_hps_io_emac1_inst_TX_CTL	(HPS_ENET_TX_EN),
        .hps_io_hps_io_emac1_inst_RX_CLK	(HPS_ENET_RX_CLK),
        .hps_io_hps_io_emac1_inst_RXD1	(HPS_ENET_RX_DATA[1]),
        .hps_io_hps_io_emac1_inst_RXD2	(HPS_ENET_RX_DATA[2]),
        .hps_io_hps_io_emac1_inst_RXD3	(HPS_ENET_RX_DATA[3]),

        // Flash
        .hps_io_hps_io_qspi_inst_IO0	(HPS_FLASH_DATA[0]),
        .hps_io_hps_io_qspi_inst_IO1	(HPS_FLASH_DATA[1]),
        .hps_io_hps_io_qspi_inst_IO2	(HPS_FLASH_DATA[2]),
        .hps_io_hps_io_qspi_inst_IO3	(HPS_FLASH_DATA[3]),
        .hps_io_hps_io_qspi_inst_SS0	(HPS_FLASH_NCSO),
        .hps_io_hps_io_qspi_inst_CLK	(HPS_FLASH_DCLK),

        // SD Card
        .hps_io_hps_io_sdio_inst_CMD	(HPS_SD_CMD),
        .hps_io_hps_io_sdio_inst_D0	(HPS_SD_DATA[0]),
        .hps_io_hps_io_sdio_inst_D1	(HPS_SD_DATA[1]),
        .hps_io_hps_io_sdio_inst_CLK	(HPS_SD_CLK),
        .hps_io_hps_io_sdio_inst_D2	(HPS_SD_DATA[2]),
        .hps_io_hps_io_sdio_inst_D3	(HPS_SD_DATA[3]),

        // USB
        .hps_io_hps_io_gpio_inst_GPIO09	(HPS_CONV_USB_N),
        .hps_io_hps_io_usb1_inst_D0		(HPS_USB_DATA[0]),
        .hps_io_hps_io_usb1_inst_D1		(HPS_USB_DATA[1]),
        .hps_io_hps_io_usb1_inst_D2		(HPS_USB_DATA[2]),
        .hps_io_hps_io_usb1_inst_D3		(HPS_USB_DATA[3]),
        .hps_io_hps_io_usb1_inst_D4		(HPS_USB_DATA[4]),
        .hps_io_hps_io_usb1_inst_D5		(HPS_USB_DATA[5]),
        .hps_io_hps_io_usb1_inst_D6		(HPS_USB_DATA[6]),
        .hps_io_hps_io_usb1_inst_D7		(HPS_USB_DATA[7]),
        .hps_io_hps_io_usb1_inst_CLK		(HPS_USB_CLKOUT),
        .hps_io_hps_io_usb1_inst_STP		(HPS_USB_STP),
        .hps_io_hps_io_usb1_inst_DIR		(HPS_USB_DIR),
        .hps_io_hps_io_usb1_inst_NXT		(HPS_USB_NXT),

        // SPI
        .hps_io_hps_io_spim1_inst_CLK		(HPS_SPIM_CLK),
        .hps_io_hps_io_spim1_inst_MOSI	(HPS_SPIM_MOSI),
        .hps_io_hps_io_spim1_inst_MISO	(HPS_SPIM_MISO),
        .hps_io_hps_io_spim1_inst_SS0		(HPS_SPIM_SS),

        // UART
        .hps_io_hps_io_uart0_inst_RX	(HPS_UART_RX),
        .hps_io_hps_io_uart0_inst_TX	(HPS_UART_TX),

        // I2C
        .hps_io_hps_io_gpio_inst_GPIO48	(HPS_I2C_CONTROL),
        .hps_io_hps_io_i2c0_inst_SDA		(HPS_I2C1_SDAT),
        .hps_io_hps_io_i2c0_inst_SCL		(HPS_I2C1_SCLK),
        .hps_io_hps_io_i2c1_inst_SDA		(HPS_I2C2_SDAT),
        .hps_io_hps_io_i2c1_inst_SCL		(HPS_I2C2_SCLK),

		// General Purpose I/O
        .hps_io_hps_io_gpio_inst_GPIO40	(HPS_GPIO[0]),
        .hps_io_hps_io_gpio_inst_GPIO41	(HPS_GPIO[1]),

		// LED
	    .hps_io_hps_io_gpio_inst_GPIO53	(HPS_LED),

		// Pushbutton
	    .hps_io_hps_io_gpio_inst_GPIO54	(HPS_KEY),
		
        // Accelerometer
	    .hps_io_hps_io_gpio_inst_GPIO61	(HPS_GSENSOR_INT),

        // DDR3 SDRAM
        .memory_mem_a			(HPS_DDR3_ADDR),
        .memory_mem_ba			(HPS_DDR3_BA),
        .memory_mem_ck			(HPS_DDR3_CK_P),
        .memory_mem_ck_n		(HPS_DDR3_CK_N),
        .memory_mem_cke		(HPS_DDR3_CKE),
        .memory_mem_cs_n		(HPS_DDR3_CS_N),
        .memory_mem_ras_n		(HPS_DDR3_RAS_N),
        .memory_mem_cas_n		(HPS_DDR3_CAS_N),
        .memory_mem_we_n		(HPS_DDR3_WE_N),
        .memory_mem_reset_n	(HPS_DDR3_RESET_N),
        .memory_mem_dq			(HPS_DDR3_DQ),
        .memory_mem_dqs		(HPS_DDR3_DQS_P),
        .memory_mem_dqs_n		(HPS_DDR3_DQS_N),
        .memory_mem_odt		(HPS_DDR3_ODT),
        .memory_mem_dm			(HPS_DDR3_DM),
        .memory_oct_rzqin		(HPS_DDR3_RZQ),

        .reset_reset(~KEY[0]),

        .touch_uart_RXD(GPIO_1[31]),            // touchscreen_uart.rxd
		.touch_uart_TXD(GPIO_1[30]),            //                 .txd
		.wifi_uart_RXD(GPIO_1[35]),                   //        wifi_uart.rxd
		.wifi_uart_TXD(GPIO_1[34]),                    //                 .txd

        .vga_CLK(VGA_CLK),
        .vga_HS(VGA_HS),
        .vga_VS(VGA_VS),
        .vga_BLANK(VGA_BLANK_N),
        .vga_SYNC(VGA_SYNC_N),
        .vga_R(VGA_R),
        .vga_G(VGA_G),
        .vga_B(VGA_B),

        .sdram_clk_clk(DRAM_CLK),
        .sdram_addr(DRAM_ADDR),
        .sdram_ba(DRAM_BA),
        .sdram_cas_n(DRAM_CAS_N),
        .sdram_cke(DRAM_CKE),
        .sdram_cs_n(DRAM_CS_N),
        .sdram_dq(DRAM_DQ),
        .sdram_dqm({DRAM_UDQM, DRAM_LDQM}),
        .sdram_ras_n(DRAM_RAS_N),
        .sdram_we_n(DRAM_WE_N),
    );
		

endmodule