module testWifiTouch (
            input logic CLOCK_50, input logic [3:0] KEY,
            input logic [9:0] SW, output logic [9:0] LEDR,
            output logic [6:0] HEX0, output logic [6:0] HEX1, output logic [6:0] HEX2,
            output logic [6:0] HEX3, output logic [6:0] HEX4, output logic [6:0] HEX5,
            inout [35:0] GPIO_0, inout [35:0] GPIO_1,

            // DRAM Pins
            output logic DRAM_CLK, output logic DRAM_CKE,
            output logic DRAM_CAS_N, output logic DRAM_RAS_N, output logic DRAM_WE_N,
            output logic [12:0] DRAM_ADDR, output logic [1:0] DRAM_BA, output logic DRAM_CS_N,
            inout logic [15:0] DRAM_DQ, output logic DRAM_UDQM, output logic DRAM_LDQM,

            // VGA Pins
            output logic [7:0] VGA_R, output logic [7:0] VGA_G, output logic [7:0] VGA_B,
            output logic VGA_HS, output logic VGA_VS, output logic VGA_CLK,
            output logic [2:0] VGA_COLOUR, output logic VGA_PLOT, 
            output logic VGA_BLANK_N, output logic VGA_SYNC_N,
				
				input logic HPS_KEY
        );
    cpu u0(
		.leds_r_export(LEDR[7:0]),  //     leds_r.export
		.touch_uart_RXD(GPIO_1[13]), // touch_uart.RXD
		.touch_uart_TXD(GPIO_1[11]), //           .TXD
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
        .sys_pll_clk_ref_clk_clk(CLOCK_50),
        .sys_pll_clk_ref_reset_reset(~KEY[0]),
        .vga_CLK(VGA_CLK),
        .vga_HS(VGA_HS),
        .vga_VS(VGA_VS),
        .vga_BLANK(VGA_BLANK_N),
        .vga_SYNC(VGA_SYNC_N),
        .vga_R(VGA_R),
        .vga_G(VGA_G),
        .vga_B(VGA_B)
	);
	assign LEDR[8] = HPS_KEY;
    assign LEDR[9] = ~KEY[0];
    assign GPIO_1[25] = 1'b1;
    assign GPIO_1[9] = 1'b1;
endmodule