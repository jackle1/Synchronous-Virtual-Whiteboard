module testWifiTouch (
            input logic CLOCK_50, input logic [3:0] KEY,
            input logic [9:0] SW, output logic [9:0] LEDR,
            output logic [6:0] HEX0, output logic [6:0] HEX1, output logic [6:0] HEX2,
            output logic [6:0] HEX3, output logic [6:0] HEX4, output logic [6:0] HEX5,
            inout [35:0] GPIO_0, inout [35:0] GPIO_1
        );
    cpu u0(
		.clk_clk(CLOCK_50),        //        clk.clk
		.leds_r_export(LEDR[7:0]),  //     leds_r.export
		.reset_reset_n(KEY[0]),  //      reset.reset_n
		.touch_uart_RXD(GPIO_1[13]), // touch_uart.RXD
		.touch_uart_TXD(GPIO_1[11]), //           .TXD
		.wifi_uart_RXD(GPIO_1[27]),  //  wifi_uart.RXD
		.wifi_uart_TXD(GPIO_1[29])   //           .TXD
	);
    assign GPIO_1[25] = 1'b1;
    assign GPIO_1[9] = 1'b1;
endmodule