	cpu u0 (
		.leds_r_export               (<connected-to-leds_r_export>),               //                leds_r.export
		.sdram_addr                  (<connected-to-sdram_addr>),                  //                 sdram.addr
		.sdram_ba                    (<connected-to-sdram_ba>),                    //                      .ba
		.sdram_cas_n                 (<connected-to-sdram_cas_n>),                 //                      .cas_n
		.sdram_cke                   (<connected-to-sdram_cke>),                   //                      .cke
		.sdram_cs_n                  (<connected-to-sdram_cs_n>),                  //                      .cs_n
		.sdram_dq                    (<connected-to-sdram_dq>),                    //                      .dq
		.sdram_dqm                   (<connected-to-sdram_dqm>),                   //                      .dqm
		.sdram_ras_n                 (<connected-to-sdram_ras_n>),                 //                      .ras_n
		.sdram_we_n                  (<connected-to-sdram_we_n>),                  //                      .we_n
		.sdram_clk_clk               (<connected-to-sdram_clk_clk>),               //             sdram_clk.clk
		.sys_pll_clk_ref_clk_clk     (<connected-to-sys_pll_clk_ref_clk_clk>),     //   sys_pll_clk_ref_clk.clk
		.sys_pll_clk_ref_reset_reset (<connected-to-sys_pll_clk_ref_reset_reset>), // sys_pll_clk_ref_reset.reset
		.touch_uart_RXD              (<connected-to-touch_uart_RXD>),              //            touch_uart.RXD
		.touch_uart_TXD              (<connected-to-touch_uart_TXD>),              //                      .TXD
		.vga_CLK                     (<connected-to-vga_CLK>),                     //                   vga.CLK
		.vga_HS                      (<connected-to-vga_HS>),                      //                      .HS
		.vga_VS                      (<connected-to-vga_VS>),                      //                      .VS
		.vga_BLANK                   (<connected-to-vga_BLANK>),                   //                      .BLANK
		.vga_SYNC                    (<connected-to-vga_SYNC>),                    //                      .SYNC
		.vga_R                       (<connected-to-vga_R>),                       //                      .R
		.vga_G                       (<connected-to-vga_G>),                       //                      .G
		.vga_B                       (<connected-to-vga_B>)                        //                      .B
	);

