module camera_top (
    
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

	//////////// CLOCK //////////
	input 		          		CLOCK2_50,
	input 		          		CLOCK3_50,
	input 		          		CLOCK4_50,
	input 		          		CLOCK_50,

	//////////// SDRAM //////////
	output		    [12:0]		DRAM_ADDR,
	output		    [1:0]		DRAM_BA,
	output		          		DRAM_CAS_N,
	output		          		DRAM_CKE,
	output		          		DRAM_CLK,
	output		          		DRAM_CS_N,
	inout 		    [15:0]		DRAM_DQ,
	output		          		DRAM_LDQM,
	output		          		DRAM_RAS_N,
	output		          		DRAM_UDQM,
	output		          		DRAM_WE_N,

	//////////// I2C for Audio and Video-In //////////
	output		          		FPGA_I2C_SCLK,
	inout 		          		FPGA_I2C_SDAT,

	//////////// SEG7 //////////
	output		     [6:0]		HEX0,
	output		     [6:0]		HEX1,
	output		     [6:0]		HEX2,
	output		     [6:0]		HEX3,
	output		     [6:0]		HEX4,
	output		     [6:0]		HEX5,

	//////////// IR //////////
	input 		          		IRDA_RXD,
	output		          		IRDA_TXD,

	//////////// KEY //////////
	input 		     [3:0]		KEY,

	//////////// LED //////////
	output reg		     [9:0]		LEDR,

	//////////// PS2 //////////
	inout 		          		PS2_CLK,
	inout 		          		PS2_CLK2,
	inout 		          		PS2_DAT,
	inout 		          		PS2_DAT2,

	//////////// SW //////////
	input 		     [9:0]		SW,

	//////////// Video-In //////////
	input 		          		TD_CLK27,
	input 		     [7:0]		TD_DATA,
	input 		          		TD_HS,
	output		          		TD_RESET_N,
	input 		          		TD_VS,

	//////////// VGA //////////
	output		          		VGA_BLANK_N,
	output		     [7:0]		VGA_B,
	output		          		VGA_CLK,
	output		     [7:0]		VGA_G,
	output		          		VGA_HS,
	output		     [7:0]		VGA_R,
	output		          		VGA_SYNC_N,
	output		          		VGA_VS,

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
	output		          		MIPI_RESET_n,
	//////////// GPIO_0, GPIO_0 connect to GPIO Default //////////
	inout 		    [35:0]		GPIO
);

    assign VGA_BLANK_N 				= 1'b1;
    assign VGA_SYNC_N 				= 1'b0;

    wire	  [23:0]	st_data;
    wire				st_valid;
    wire				st_sop;
    wire		 		st_eop;
    wire				st_ready;

//    camera_module  (
//		.alt_vip_cl_cvo_0_clocked_video_vid_clk(~VGA_CLK),       // alt_vip_cl_cvo_0_clocked_video.vid_clk
//		.alt_vip_cl_cvo_0_clocked_video_vid_data({VGA_R, VGA_G, VGA_B}),      //                               .vid_data
//		.alt_vip_cl_cvo_0_clocked_video_underflow(),     //                               .underflow
//		.alt_vip_cl_cvo_0_clocked_video_vid_datavalid(), //                               .vid_datavalid
//		.alt_vip_cl_cvo_0_clocked_video_vid_v_sync(VGA_VS),    //                               .vid_v_sync
//		.alt_vip_cl_cvo_0_clocked_video_vid_h_sync(VGA_HS),    //                               .vid_h_sync
//		.alt_vip_cl_cvo_0_clocked_video_vid_f(),         //                               .vid_f
//		.alt_vip_cl_cvo_0_clocked_video_vid_h(),         //                               .vid_h
//		.alt_vip_cl_cvo_0_clocked_video_vid_v(),         //                               .vid_v
//		.alt_vip_cl_vfb_0_din_data(st_data),                    //           alt_vip_cl_vfb_0_din.data
//		.alt_vip_cl_vfb_0_din_valid(st_valid),                   //                               .valid
//		.alt_vip_cl_vfb_0_din_startofpacket(st_sop),           //                               .startofpacket
//		.alt_vip_cl_vfb_0_din_endofpacket(st_eop),             //                               .endofpacket
//		.alt_vip_cl_vfb_0_din_ready(st_ready),                   //                               .ready
//		.clk_clk(CLOCK_50),                                      //                            clk.clk
//		.pio_in_export(KEY[2]),                                //                         pio_in.export
//		.reset_reset_n(KEY[0]),                                //                          reset.reset_n
//		.sdram_addr(DRAM_ADDR),                                   //                          sdram.addr
//		.sdram_ba(DRAM_BA),                                     //                               .ba
//		.sdram_cas_n(DRAM_CAS_N),                                  //                               .cas_n
//		.sdram_cke(DRAM_CKE),                                    //                               .cke
//		.sdram_cs_n(DRAM_CS_N),                                   //                               .cs_n
//		.sdram_dq(DRAM_DQ),                                     //                               .dq
//		.sdram_dqm({DRAM_UDQM, DRAM_LDQM}),                                    //                               .dqm
//		.sdram_ras_n(DRAM_RAS_N),                                  //                               .ras_n
//		.sdram_we_n(DRAM_WE_N)                                    //                               .we_n
//	);



     DE1_SOC_D8M_RTL camera(

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

 	//////////// CLOCK //////////
 	input 		          		CLOCK2_50,
 	input 		          		CLOCK3_50,
 	input 		          		CLOCK4_50,
 	input 		          		CLOCK_50,

 	//////////// SDRAM //////////
 	output		    [12:0]		DRAM_ADDR,
 	output		    [1:0]		DRAM_BA,
 	output		          		DRAM_CAS_N,
 	output		          		DRAM_CKE,
 	output		          		DRAM_CLK,
 	output		          		DRAM_CS_N,
 	inout 		    [15:0]		DRAM_DQ,
 	output		          		DRAM_LDQM,
 	output		          		DRAM_RAS_N,
 	output		          		DRAM_UDQM,
 	output		          		DRAM_WE_N,

 	//////////// I2C for Audio and Video-In //////////
 	output		          		FPGA_I2C_SCLK,
 	inout 		          		FPGA_I2C_SDAT,

 	//////////// SEG7 //////////
 	output		     [6:0]		HEX0,
 	output		     [6:0]		HEX1,
 	output		     [6:0]		HEX2,
 	output		     [6:0]		HEX3,
 	output		     [6:0]		HEX4,
 	output		     [6:0]		HEX5,

 	//////////// IR //////////
 	input 		          		IRDA_RXD,
 	output		          		IRDA_TXD,

 	//////////// KEY //////////
 	input 		     [3:0]		KEY,

 	//////////// LED //////////
 	output reg		     [9:0]		LEDR,

 	//////////// PS2 //////////
 	inout 		          		PS2_CLK,
 	inout 		          		PS2_CLK2,
 	inout 		          		PS2_DAT,
 	inout 		          		PS2_DAT2,

 	//////////// SW //////////
 	input 		     [9:0]		SW,

 	//////////// Video-In //////////
 	input 		          		TD_CLK27,
 	input 		     [7:0]		TD_DATA,
 	input 		          		TD_HS,
 	output		          		TD_RESET_N,
 	input 		          		TD_VS,

 	//////////// VGA //////////
 	output		          		VGA_BLANK_N,
 	output		     [7:0]		VGA_B,
 	output		          		VGA_CLK,
 	output		     [7:0]		VGA_G,
 	output		          		VGA_HS,
 	output		     [7:0]		VGA_R,
 	output		          		VGA_SYNC_N,
 	output		          		VGA_VS,

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
 	output		          		MIPI_RESET_n,
 	//////////// GPIO_0, GPIO_0 connect to GPIO Default //////////
 	inout 		    [35:0]		GPIO,

 	output reg					img_captured,

 	//FIFO RD 2
 	output [9:0]				rd2_pixel_data,   
 	input						rd2_req,
 	input [`ASIZE-1:0]			rd2_start_addr,
 	input [`ASIZE-1:0]			rd2_max_addr,
 	input [10:0]				rd2_len,

 	//FIFO WR2
 	// input [15:0] 				wr2_in_data,
 	// input 					wr2_req,
 	output reg [15:0] 				wr2_in_data,
 	output reg [15:0] 				wr2_in_data2,
 	output reg					wr2_req,

 	output 						sdram_oe,

 	input 						vga2_req,

 	output	  [23:0]	st_data,
 	output				st_valid,
 	output				st_sop,
 	output		 		st_eop,
 	input				st_ready	//TODO: Uncomment
	
 );

endmodule