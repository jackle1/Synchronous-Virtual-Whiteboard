	camera_module u0 (
		.clk_clk                                    (<connected-to-clk_clk>),                                    //                           clk.clk
		.img_cpu_reader_0_cpu_rdy_cpu_rdy           (<connected-to-img_cpu_reader_0_cpu_rdy_cpu_rdy>),           //      img_cpu_reader_0_cpu_rdy.cpu_rdy
		.img_cpu_reader_0_get_next_pix_get_next_pix (<connected-to-img_cpu_reader_0_get_next_pix_get_next_pix>), // img_cpu_reader_0_get_next_pix.get_next_pix
		.img_cpu_reader_0_img_done_img_done         (<connected-to-img_cpu_reader_0_img_done_img_done>),         //     img_cpu_reader_0_img_done.img_done
		.img_cpu_reader_0_out_state_out_state       (<connected-to-img_cpu_reader_0_out_state_out_state>),       //    img_cpu_reader_0_out_state.out_state
		.img_cpu_reader_0_pix_data_pixel_data       (<connected-to-img_cpu_reader_0_pix_data_pixel_data>),       //     img_cpu_reader_0_pix_data.pixel_data
		.img_cpu_reader_0_pix_rdy_pix_rdy           (<connected-to-img_cpu_reader_0_pix_rdy_pix_rdy>),           //      img_cpu_reader_0_pix_rdy.pix_rdy
		.img_cpu_reader_0_pix_rdy_out_pix_rdy_out   (<connected-to-img_cpu_reader_0_pix_rdy_out_pix_rdy_out>),   //  img_cpu_reader_0_pix_rdy_out.pix_rdy_out
		.img_cpu_reader_0_pix_rgb_out_pix_rgb_out   (<connected-to-img_cpu_reader_0_pix_rgb_out_pix_rgb_out>),   //  img_cpu_reader_0_pix_rgb_out.pix_rgb_out
		.pio_in_export                              (<connected-to-pio_in_export>),                              //                        pio_in.export
		.reset_reset_n                              (<connected-to-reset_reset_n>)                               //                         reset.reset_n
	);

