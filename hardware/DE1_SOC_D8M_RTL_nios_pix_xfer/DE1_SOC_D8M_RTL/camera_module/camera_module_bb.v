
module camera_module (
	clk_clk,
	img_cpu_reader_0_cpu_rdy_cpu_rdy,
	img_cpu_reader_0_get_next_pix_get_next_pix,
	img_cpu_reader_0_img_done_img_done,
	img_cpu_reader_0_out_state_out_state,
	img_cpu_reader_0_pix_data_pixel_data,
	img_cpu_reader_0_pix_rdy_pix_rdy,
	img_cpu_reader_0_pix_rdy_out_pix_rdy_out,
	img_cpu_reader_0_pix_rgb_out_pix_rgb_out,
	pio_in_export,
	reset_reset_n);	

	input		clk_clk;
	output		img_cpu_reader_0_cpu_rdy_cpu_rdy;
	output		img_cpu_reader_0_get_next_pix_get_next_pix;
	input		img_cpu_reader_0_img_done_img_done;
	output	[3:0]	img_cpu_reader_0_out_state_out_state;
	input	[23:0]	img_cpu_reader_0_pix_data_pixel_data;
	input		img_cpu_reader_0_pix_rdy_pix_rdy;
	output	[31:0]	img_cpu_reader_0_pix_rdy_out_pix_rdy_out;
	output	[23:0]	img_cpu_reader_0_pix_rgb_out_pix_rgb_out;
	input		pio_in_export;
	input		reset_reset_n;
endmodule
