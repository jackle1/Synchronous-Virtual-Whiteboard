
module cpu (
	clk_clk,
	reset_reset_n,
	wifi_uart_RXD,
	wifi_uart_TXD,
	touch_uart_RXD,
	touch_uart_TXD,
	leds_r_export);	

	input		clk_clk;
	input		reset_reset_n;
	input		wifi_uart_RXD;
	output		wifi_uart_TXD;
	input		touch_uart_RXD;
	output		touch_uart_TXD;
	output	[7:0]	leds_r_export;
endmodule
