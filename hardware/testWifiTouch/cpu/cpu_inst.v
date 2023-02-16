	cpu u0 (
		.clk_clk        (<connected-to-clk_clk>),        //        clk.clk
		.reset_reset_n  (<connected-to-reset_reset_n>),  //      reset.reset_n
		.wifi_uart_RXD  (<connected-to-wifi_uart_RXD>),  //  wifi_uart.RXD
		.wifi_uart_TXD  (<connected-to-wifi_uart_TXD>),  //           .TXD
		.touch_uart_RXD (<connected-to-touch_uart_RXD>), // touch_uart.RXD
		.touch_uart_TXD (<connected-to-touch_uart_TXD>), //           .TXD
		.leds_r_export  (<connected-to-leds_r_export>)   //     leds_r.export
	);

