// https://people.ece.cornell.edu/land/courses/ece5760/DE1_SOC/HPS_FPGA/pio_test/ghrd_top.v
module hps_system (
       //////////// ADC //////////
	output		          		ADC_CONVST,
	output		          		ADC_DIN,
	input 		          		ADC_DOUT,
	output		          		ADC_SCLK,

	//////////// Audio //////////
	input 		          		AUD_ADCDAT,
	inout 		          		AUD_ADCLRCK,
	inout 		          		AUD_BCLK,
	output		          		AUD_DACDAT,
	inout 		          		AUD_DACLRCK,
	output		          		AUD_XCK,

      ///////// CLOCK /////////
      input 		          		CLOCK2_50,
      input 		          		CLOCK3_50,
      input 		          		CLOCK4_50,
      input 		          		CLOCK_50,

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

      //////////// IR //////////
	  input 		          		IRDA_RXD,
	  output		          		IRDA_TXD,

      ///////// KEY /////////
      input       [3:0]  KEY,

      ///////// LEDR /////////
      output      [9:0]  LEDR,

      //////////// PS2 //////////
	inout 		          		PS2_CLK,
	inout 		          		PS2_CLK2,
	inout 		          		PS2_DAT,
	inout 		          		PS2_DAT2,

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
      output             VGA_VS,

      //////////// GPIO_1, GPIO_1 connect to D8M-GPIO //////////
	output 		          		CAMERA_I2C_SCL,
	inout 		          		CAMERA_I2C_SDA,
	output		          		CAMERA_PWDN_n,
	output		          		MIPI_CS_n,
	inout 		          		MIPI_I2C_SCL,
	inout 		          		MIPI_I2C_SDA,
	output		          		MIPI_MCLK,
	input 		          		MIPI_PIXEL_CLK,
	input 		     [9:0]		MIPI_PIXEL_D,
	input 		          		MIPI_PIXEL_HS,
	input 		          		MIPI_PIXEL_VS,
	output		          		MIPI_REFCLK,
	output		          		MIPI_RESET_n
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

        .touch_uart_RXD(GPIO_0[31]),            // touchscreen_uart.rxd
		.touch_uart_TXD(GPIO_0[30]),            //                 .txd

        .vga_CLK(hps_vga_clk),
        .vga_HS(hps_vga_hs),
        .vga_VS(hps_vga_vs),
        .vga_BLANK(hps_vga_blank_n),
        .vga_SYNC(hps_vga_sync_n),
        .vga_R(hps_vga_r),
        .vga_G(hps_vga_g),
        .vga_B(hps_vga_b),

        .sdram_clk_clk(hps_dram_clk),
        .sdram_addr(hps_dram_addr),
        .sdram_ba(hps_dram_ba),
        .sdram_cas_n(hps_dram_cas_n),
        .sdram_cke(hps_dram_cke),
        .sdram_cs_n(hps_dram_cs_n),
        .sdram_dq(DRAM_DQ),
        .sdram_dqm({hps_dram_udqm, hps_dram_ldqm}),
        .sdram_ras_n(hps_dram_ras_n),
        .sdram_we_n(hps_dram_we_n),
        .buttons_export(KEY),
        .switches_export(SW),
        .hexes_export({cpu_hex3, cpu_hex2, cpu_hex1, cpu_hex0}),

        //img_cpu_reader
        .img_cpu_reader_0_cpu_rdy_cpu_rdy(cpu_pix_rdy),
		.img_cpu_reader_0_get_next_pix_get_next_pix(get_next_pix),
		.img_cpu_reader_0_img_done_img_done(img_rd_done),
		.img_cpu_reader_0_out_state_out_state(out_state), 
		.img_cpu_reader_0_pix_rdy_pix_rdy(pixel_ready), 
		.img_cpu_reader_0_pix_rdy_out_pix_rdy_out(pix_rdy_out), 
		.img_cpu_reader_0_pix_rgb_out_pix_rgb_out(pix_rgb_out), 
		.img_cpu_reader_0_pixel_data_pixel_data({img_r, img_g, img_b}), 
    );

    wire cpu_pix_rdy;
    wire [3:0] out_state;   //for debugging only
    wire [31:0] pix_rdy_out;   //for debugging only
    wire [23:0] pix_rgb_out;   //for debugging only
    wire img_captured;

    DE1_SOC_D8M_RTL camera (
        .ADC_CONVST(ADC_CONVST),
	    .ADC_DIN(ADC_DIN),
	    .ADC_DOUT(ADC_DOUT),
	    .ADC_SCLK(ADC_SCLK),

        .AUD_ADCDAT(AUD_ADCDAT),
        .AUD_ADCLRCK(AUD_ADCLRCK),
        .AUD_BCLK(AUD_BCLK),
        .AUD_DACDAT(AUD_DACDAT),
        .AUD_DACLRCK(AUD_DACLRCK),
        .AUD_XCK(AUD_XCK),

        .CLOCK2_50(CLOCK2_50),
        .CLOCK3_50(CLOCK3_50),
        .CLOCK4_50(CLOCK4_50),
        .CLOCK_50(CLOCK_50),

        .DRAM_ADDR(camera_dram_addr),
	    .DRAM_BA(camera_dram_ba),
	    .DRAM_CAS_N(camera_dram_cas_n),
	    .DRAM_CKE(camera_dram_cke),
	    .DRAM_CLK(camera_dram_clk),
	    .DRAM_CS_N(camera_dram_cs_n),
	    .DRAM_DQ(DRAM_DQ),
        .DRAM_LDQM(camera_dram_ldqm),
        .DRAM_RAS_N(camera_dram_ras_n),
	    .DRAM_UDQM(camera_dram_udqm),
	    .DRAM_WE_N(camera_dram_we_n),

        .FPGA_I2C_SCLK(FPGA_I2C_SCLK),
        .FPGA_I2C_SDAT(FPGA_I2C_SDAT),

        .GPIO(GPIO_0),

        .HEX0(cam_hex0),
        .HEX1(cam_hex1),
        .HEX2(cam_hex2),
        .HEX3(cam_hex3),
        .HEX4(cam_hex4),
        .HEX5(cam_hex5),

        .IRDA_RXD(IRDA_RXD),
        .IRDA_TXD(IRDA_TXD),

        .KEY(KEY),

        .LEDR(LEDR),

        .PS2_CLK(PS2_CLK),
        .PS2_CLK2(PS2_CLK2),
        .PS2_DAT(PS2_DAT),
        .PS2_DAT2(PS2_DAT2),

        .SW(SW),

        .TD_CLK27(TD_CLK27),
        .TD_DATA(TD_DATA),
        .TD_HS(TD_HS),
        .TD_RESET_N(TD_RESET_N),
        .TD_VS(TD_VS),

        .VGA_BLANK_N(camera_vga_blank_n),
        .VGA_B(camera_vga_b),
        .VGA_CLK(camera_vga_clk),
        .VGA_G(camera_vga_g),
        .VGA_HS(camera_vga_hs),
        .VGA_R(camera_vga_r),
        .VGA_SYNC_N(camera_vga_sync_n),
        .VGA_VS(camera_vga_vs),

        .CAMERA_I2C_SCL(CAMERA_I2C_SCL),
        .CAMERA_I2C_SDA(CAMERA_I2C_SDA),
        .CAMERA_PWDN_n(CAMERA_PWDN_n),
        .MIPI_CS_n(MIPI_CS_n),
	    .MIPI_I2C_SCL(MIPI_I2C_SCL),
	    .MIPI_I2C_SDA(MIPI_I2C_SDA),
	    .MIPI_MCLK(MIPI_MCLK),
	    .MIPI_PIXEL_CLK(MIPI_PIXEL_CLK),
	    .MIPI_PIXEL_D(MIPI_PIXEL_D),
	    .MIPI_PIXEL_HS(MIPI_PIXEL_HS),
	    .MIPI_PIXEL_VS(MIPI_PIXEL_VS),
	    .MIPI_REFCLK(MIPI_REFCLK),
	    .MIPI_RESET_n(MIPI_RESET_n),

        .img_captured(img_captured),
        .rd2_pixel_data(rd2_pixel_data),
        .rd2_req(rd2_req),
        .rd2_start_addr(PIC_START_ADDR),
        .rd2_max_addr(PIC_START_ADDR + 640*480),
        .rd2_len(256)
    );

    assign cpu_hex4 = 7'h7F;
    assign cpu_hex5 = 7'h7F;

    //----Multiplex between VGA and Camera----
//DE1 VGA output
wire [7:0] camera_vga_r, camera_vga_g, camera_vga_b, hps_vga_r, hps_vga_g, hps_vga_b;
wire camera_vga_hs, camera_vga_vs, hps_vga_hs, hps_vga_vs;
wire camera_vga_blank_n, camera_vga_sync_n, hps_vga_blank_n, hps_vga_sync_n;
wire camera_vga_clk, hps_vga_clk;

assign {VGA_R, VGA_G, VGA_B} = SW[8] ? {camera_vga_r, camera_vga_g, camera_vga_b} : {hps_vga_r, hps_vga_g, hps_vga_b};
assign {VGA_HS, VGA_VS} = SW[8] ? {camera_vga_hs, camera_vga_vs} : {hps_vga_hs, hps_vga_vs};
assign {VGA_BLANK_N, VGA_SYNC_N} = SW[8] ? {camera_vga_blank_n, camera_vga_sync_n} : {hps_vga_blank_n, hps_vga_sync_n};
assign VGA_CLK = SW[8] ? camera_vga_clk : hps_vga_clk;

// VGA Multiplex working on NIOS
// assign {VGA_R, VGA_G, VGA_B} = SW[8] ? {camera_vga_r, camera_vga_g, camera_vga_b} : 24'h7FF;
// assign {VGA_HS, VGA_VS} = SW[8] ? {camera_vga_hs, camera_vga_vs} : 2'b01;
// assign {VGA_BLANK_N, VGA_SYNC_N} = SW[8] ? {camera_vga_blank_n, camera_vga_sync_n} : 2'b01;
// assign VGA_CLK = SW[8] ? camera_vga_clk : 1'b1;

//DE1 DRAM Output
wire[12:0] camera_dram_addr, hps_dram_addr;
wire[1:0] camera_dram_ba, hps_dram_ba;
wire[15:0] camera_dq, dram_dq_interconnect, hps_dram_dq;
wire camera_dram_cas_n, camera_dram_cke, camera_dram_cs_n, camera_dram_ldqm, camera_dram_ras_n, camera_dram_udqm, camera_dram_we_n, hps_dram_cas_n, hps_dram_cke, hps_dram_cs_n, hps_dram_ldqm, hps_dram_ras_n, hps_dram_udqm, hps_dram_we_n;
wire camera_dram_clk, hps_dram_clk; 

assign DRAM_ADDR = SW[8] ? camera_dram_addr : hps_dram_addr;
assign DRAM_BA = SW[8] ? camera_dram_ba : hps_dram_ba;
assign {DRAM_CAS_N, DRAM_CKE, DRAM_CS_N, DRAM_LDQM, DRAM_RAS_N, DRAM_UDQM, DRAM_WE_N} = SW[8] ? {camera_dram_cas_n, camera_dram_cke, camera_dram_cs_n, camera_dram_ldqm, camera_dram_ras_n, camera_dram_udqm, camera_dram_we_n} : {hps_dram_cas_n, hps_dram_cke, hps_dram_cs_n, hps_dram_ldqm, hps_dram_ras_n, hps_dram_udqm, hps_dram_we_n};
assign DRAM_CLK = SW[8] ? camera_dram_clk : hps_dram_clk;

wire [6:0] cam_hex0, cam_hex1, cam_hex2, cam_hex3, cam_hex4, cam_hex5, cam_hex6;
wire [6:0] cpu_hex0, cpu_hex1, cpu_hex2, cpu_hex3, cpu_hex4, cpu_hex5, cpu_hex6;
assign {HEX6, HEX5, HEX4, HEX3, HEX2, HEX1, HEX0} = SW[8] ? {cam_hex0, cam_hex1, cam_hex2, cam_hex3, cam_hex4, cam_hex5, cam_hex6} : {cpu_hex0, cpu_hex1, cpu_hex2, cpu_hex3, cpu_hex4, cpu_hex5, cpu_hex6};

wire get_next_pix;  //cpu_rdy on img_cpu_reader
wire [7:0] img_r, img_g, img_b;
wire img_rd_done, pixel_ready;
wire [3:0] curr_state;   //debugging purposes
wire [8:0] loop_count;  //debugging purposes
wire rd2_req;  //req next pixel from D8M read fifo 2
wire [9:0] rd2_pixel_data;   //= img_cpu_reader sdram_data

img_cpu_reader sdram_pix_reader (
	.clk(CLOCK2_50),
	.img_captured(img_captured),
    .ack(get_next_pix),
    .pixel_rdy(pixel_ready),
    .img_done(img_rd_done),
    .RED2(img_r),
    .GREEN2(img_g),
    .BLUE2(img_b),
    .rd2_data(rd2_pixel_data),
    .rd2_req(rd2_req),
    .start_addr(PIC_START_ADDR),
    .max_addr(PIC_START_ADDR + 640*480),
    .rd_len(256),
	.curr_state(curr_state),
	.rst_n(KEY[0]),
	.send_img(KEY[3]),
	.loop_count(loop_count)
);


endmodule