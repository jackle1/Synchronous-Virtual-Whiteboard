
//=======================================================
//  This code is generated by Terasic System Builder
//=======================================================
`define PIC_START_ADDR 32'h0 //(32'h0200_0000)
module DE1_SOC_D8M_RTL(

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
//=============================================================================
// REG/WIRE declarations
//=============================================================================


wire	[15:0]SDRAM_RD_DATA;
wire			DLY_RST_0;
wire			DLY_RST_1;
wire			DLY_RST_2;

wire			SDRAM_CTRL_CLK;
wire        D8M_CK_HZ ; 
wire        D8M_CK_HZ2 ; 
wire        D8M_CK_HZ3 ; 

wire [7:0] RED; 
wire [7:0] GREEN; 
wire [7:0] BLUE; 
wire [12:0] VGA_H_CNT;			
wire [12:0] VGA_V_CNT;	

wire        READ_Request ;
wire 	[7:0] B_AUTO;
wire 	[7:0] G_AUTO;
wire 	[7:0] R_AUTO;
wire        RESET_N  ; 

wire        I2C_RELEASE ;  
wire        AUTO_FOC ; 
wire        CAMERA_I2C_SCL_MIPI ; 
wire        CAMERA_I2C_SCL_AF;
wire        CAMERA_MIPI_RELAESE ;
wire        MIPI_BRIDGE_RELEASE ;  
 
wire        LUT_MIPI_PIXEL_HS;
wire        LUT_MIPI_PIXEL_VS;
wire [9:0]  LUT_MIPI_PIXEL_D  ;
wire        MIPI_PIXEL_CLK_; 
wire [9:0]  PCK;

reg stop_capturing_nxt_frame;
reg capture;
reg pre_vs;
//=======================================================
// Structural coding
//=======================================================
//--INPU MIPI-PIXEL-CLOCK DELAY
CLOCK_DELAY  del1(  .iCLK (MIPI_PIXEL_CLK),  .oCLK (MIPI_PIXEL_CLK_ ) );


assign LUT_MIPI_PIXEL_HS=MIPI_PIXEL_HS;
assign LUT_MIPI_PIXEL_VS=MIPI_PIXEL_VS;
assign LUT_MIPI_PIXEL_D =MIPI_PIXEL_D ;

//------UART OFF --
assign UART_RTS =0; 
assign UART_TXD =0; 
//------HEX OFF --
assign HEX2           = 7'h7F;
assign HEX3           = 7'h7F;
assign HEX4           = 7'h7F;
assign HEX5           = 7'h7F;

//------ MIPI BRIGE & CAMERA RESET  --
assign CAMERA_PWDN_n  = 1; 
assign MIPI_CS_n      = 0; 
assign MIPI_RESET_n   = RESET_N ;

//------ CAMERA MODULE I2C SWITCH  --
assign I2C_RELEASE    = CAMERA_MIPI_RELAESE & MIPI_BRIDGE_RELEASE; 
assign CAMERA_I2C_SCL =( I2C_RELEASE  )?  CAMERA_I2C_SCL_AF  : CAMERA_I2C_SCL_MIPI ;   
 
//----- RESET RELAY  --		
RESET_DELAY			u2	(	
							.iRST  ( KEY[0] ),
                     .iCLK  ( CLOCK2_50 ),
							.oRST_0( DLY_RST_0 ),
							.oRST_1( DLY_RST_1 ),
							.oRST_2( DLY_RST_2 ),					
						   .oREADY( RESET_N)  
							
						);
 
//------ MIPI BRIGE & CAMERA SETTING  --  
MIPI_BRIDGE_CAMERA_Config    cfin(
                      .RESET_N           ( RESET_N ), 
                      .CLK_50            ( CLOCK2_50 ), 
                      .MIPI_I2C_SCL      ( MIPI_I2C_SCL ), 
                      .MIPI_I2C_SDA      ( MIPI_I2C_SDA ), 
                      .MIPI_I2C_RELEASE  ( MIPI_BRIDGE_RELEASE ),  
                      .CAMERA_I2C_SCL    ( CAMERA_I2C_SCL_MIPI ),
                      .CAMERA_I2C_SDA    ( CAMERA_I2C_SDA ),
                      .CAMERA_I2C_RELAESE( CAMERA_MIPI_RELAESE )
             );
				 
//------MIPI / VGA REF CLOCK  --
pll_test pll_ref(
	                   .inclk0 ( CLOCK3_50 ),
	                   .areset ( ~KEY[0]   ),
	                   .c0( MIPI_REFCLK    ) //20Mhz

    );
	 
//------MIPI / VGA REF CLOCK  -
VIDEO_PLL pll_ref1(
	                   .inclk0 ( CLOCK2_50 ),
	                   .areset ( ~KEY[0] ),
	                   .c0( VGA_CLK )        //25 Mhz	
    );	 
//------SDRAM CLOCK GENNERATER  --
sdram_pll u6(
		               .areset( 0 ) ,     
		               .inclk0( CLOCK_50 ),              
		               .c1    ( DRAM_CLK ),       //100MHZ   -90 degree
		               .c0    ( SDRAM_CTRL_CLK )  //100MHZ     0 degree 							
		              
	               );

always @(posedge CLOCK_50) begin

end

always @(posedge CLOCK_50) begin

end

wire [9:0] rd2_pixel_data;   //= img_cpu_reader sdram_data
wire rd2_req;   //== img_cpu_reader rd_req
wire rd2_clear;   //== rd_fifo_clear

//------SDRAM CONTROLLER --
Sdram_Control	   u7	(	//	HOST Side						
						   .RESET_N     ( KEY[0] ),
							.CLK         ( SDRAM_CTRL_CLK ) , 
							//	FIFO Write Side 1
							.WR1_DATA    ( LUT_MIPI_PIXEL_D[9:0] ),
							 .WR1         ( capture & LUT_MIPI_PIXEL_HS & LUT_MIPI_PIXEL_VS ) ,
							
							.WR1_ADDR    ( PIC_START_ADDR ),
                     .WR1_MAX_ADDR( PIC_START_ADDR + 640*480 ),
						   .WR1_LENGTH  ( 256 ) , 
		               .WR1_LOAD    ( !DLY_RST_0),
							.WR1_CLK     ( MIPI_PIXEL_CLK_),

                     //	FIFO Read Side 1
						   .RD1_DATA    ( SDRAM_RD_DATA[9:0] ),
				        	.RD1         ( READ_Request ),
				        	.RD1_ADDR    (PIC_START_ADDR ),
                     .RD1_MAX_ADDR( PIC_START_ADDR + 640*480 ),
							.RD1_LENGTH  ( 256  ),
							.RD1_LOAD    ( !DLY_RST_1 ),
							.RD1_CLK     ( VGA_CLK ),

							//	FIFO Read Side 2
							.RD2_DATA(rd2_pixel_data),
							.RD2(rd2_req),
							.RD2_ADDR(PIC_START_ADDR),
							.RD2_MAX_ADDR(PIC_START_ADDR + 640*480),
							.RD2_LENGTH(256),
							.RD2_LOAD(rd2_clear),
							.RD2_CLK(CLOCK_50),
											
							//	SDRAM Side
						   .SA          ( DRAM_ADDR ),
							.BA          ( DRAM_BA ),
							.CS_N        ( DRAM_CS_N ),
							.CKE         ( DRAM_CKE ),
							.RAS_N       ( DRAM_RAS_N ),
							.CAS_N       ( DRAM_CAS_N ),
							.WE_N        ( DRAM_WE_N ),
							.DQ          ( DRAM_DQ ),
							.DQM         ( DRAM_DQM  )
						   );	 	 
	 
//------ CMOS CCD_DATA TO RGB_DATA -- 

RAW2RGB_J				u4	(	
							.RST          ( VGA_VS ),
							.iDATA        ( SDRAM_RD_DATA[9:0] ),

							//-----------------------------------
							.VGA_CLK      ( VGA_CLK ),
							.READ_Request ( READ_Request ),
							.VGA_VS       ( VGA_VS ),	
							.VGA_HS       ( VGA_HS ) , 
	                  			
							.oRed         ( RED  ),
							.oGreen       ( GREEN),
							.oBlue        ( BLUE )
							);



//------AOTO FOCUS ENABLE  --
AUTO_FOCUS_ON  vd( 
                      .CLK_50      ( CLOCK2_50 ), 
                      .I2C_RELEASE ( I2C_RELEASE ), 
                      .AUTO_FOC    ( AUTO_FOC )
               ) ;
					

//------AOTO FOCUS ADJ  --
FOCUS_ADJ adl(
                      .CLK_50        ( CLOCK2_50 ) , 
                      .RESET_N       ( I2C_RELEASE ), 
                      .RESET_SUB_N   ( I2C_RELEASE ), 
                      .AUTO_FOC      ( KEY[3] & AUTO_FOC ), 
                      .SW_Y          ( 0 ),
                      .SW_H_FREQ     ( 0 ),   
                      .SW_FUC_LINE   ( SW[3] ),   
                      .SW_FUC_ALL_CEN( SW[3] ),
                      .VIDEO_HS      ( VGA_HS ),
                      .VIDEO_VS      ( VGA_VS ),
                      .VIDEO_CLK     ( VGA_CLK ),
		                .VIDEO_DE      (READ_Request) ,
                      .iR            ( R_AUTO ), 
                      .iG            ( G_AUTO ), 
                      .iB            ( B_AUTO ), 
                      .oR            ( VGA_R ) , 
                      .oG            ( VGA_G ) , 
                      .oB            ( VGA_B ) , 
                      
                      .READY         ( READY ),
                      .SCL           ( CAMERA_I2C_SCL_AF ), 
                      .SDA           ( CAMERA_I2C_SDA )
);

//------VGA Controller  --

VGA_Controller		u1	(	//	Host Side
							 .oRequest( READ_Request ),
							 .iRed    ( RED    ),
							 .iGreen  ( GREEN  ),
							 .iBlue   ( BLUE   ),
							 
							 //	VGA Side
							 .oVGA_R  ( R_AUTO[7:0] ),
							 .oVGA_G  ( G_AUTO[7:0] ),
							 .oVGA_B  ( B_AUTO[7:0] ),
							 .oVGA_H_SYNC( VGA_HS ),
							 .oVGA_V_SYNC( VGA_VS ),
							 .oVGA_SYNC  ( VGA_SYNC_N ),
							 .oVGA_BLANK ( VGA_BLANK_N ),
							 //	Control Signal
							 .iCLK       ( VGA_CLK ),
							 .iRST_N     ( DLY_RST_2 ),
							 .H_Cont     ( VGA_H_CNT ),						
						    .V_Cont     ( VGA_V_CNT )								
		);	


wire img_rd_done, pixel_ready, cpu_rdy;
reg img_captured;
wire [7:0] img_r, img_g, img_b;

wire [3:0] curr_state;

wire [8:0] loop_count;

// assign cpu_rdy = ~KEY[2];

img_cpu_reader sdram_pix_reader (
	.clk(CLOCK2_50),
	.img_captured(img_captured),
    .ack(cpu_rdy),
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

wire [3:0] out_state;
wire [31:0] pix_rdy_out;
wire [23:0] pix_rgb_out;
wire cpu_pix_rdy;

camera_module img_reader(
		.clk_clk(CLOCK_50),                                    //                           clk.clk
		.img_cpu_reader_0_cpu_rdy_cpu_rdy(cpu_pix_rdy),           //      img_cpu_reader_0_cpu_rdy.cpu_rdy
		.img_cpu_reader_0_get_next_pix_get_next_pix(cpu_rdy), // img_cpu_reader_0_get_next_pix.get_next_pix
		.img_cpu_reader_0_img_done_img_done(img_rd_done),         //     img_cpu_reader_0_img_done.img_done
		.img_cpu_reader_0_out_state_out_state(out_state),       //    img_cpu_reader_0_out_state.out_state
		.img_cpu_reader_0_pix_data_pixel_data({img_r, img_g, img_b}),       //     img_cpu_reader_0_pix_data.pixel_data
		.img_cpu_reader_0_pix_rdy_pix_rdy(pixel_ready),           //      img_cpu_reader_0_pix_rdy.pix_rdy
		.img_cpu_reader_0_pix_rdy_out_pix_rdy_out(pix_rdy_out),   //  img_cpu_reader_0_pix_rdy_out.pix_rdy_out
		.img_cpu_reader_0_pix_rgb_out_pix_rgb_out(pix_rgb_out),   //  img_cpu_reader_0_pix_rgb_out.pix_rgb_out
		.reset_reset_n(KEY[0]),                               //                         reset.reset_n
		.pio_in_export(~KEY[2])
	);

//always @(posedge CLOCK2_50) begin
//	LEDR[9] <= img_captured;
//	LEDR[8] <= pixel_ready;
//	LEDR[7] <= cpu_rdy_out;
//	LEDR[6] <= img_rd_done;
//	// LEDR[6:5] <= cpu_rdy_out[1:0];
//	LEDR[3:0] <= curr_state;
//end

// Display rgb and states on LED for debug
always @(posedge CLOCK2_50) begin
	// LEDR[7:0] <= sdram_rd_data[7:0];
	// LEDR[8:4] <= loop_count;
	LEDR[9] <= img_captured;
	if(SW[5])
		LEDR[7:0] <= pix_rgb_out[23:16];
	else if(SW[4])
		LEDR[7:0] <= pix_rgb_out[15:8];
	else if(SW[3])
		LEDR[7:0] <= pix_rgb_out[7:0];
	else if(SW[2])
		LEDR[7:0] <= img_r;
	else if (SW[1])
		LEDR[7:0] <= img_g;
	else if (SW[0])
		LEDR[7:0] <= img_b;
	else begin
		LEDR[3:0] <= curr_state;
		LEDR[7:4] <= out_state;
		// LEDR[8:4] <= loop_count;
	end
end


//----Image Capture----

 always @(posedge CLOCK2_50) begin
 	pre_vs <= LUT_MIPI_PIXEL_VS;
 	if(~SW[9] && {pre_vs,LUT_MIPI_PIXEL_VS} == 2'b01) begin    //SW off at start of new frame - don't capture
 		capture <= 0;
 	end else if(~SW[9] && capture == 1 && {pre_vs,LUT_MIPI_PIXEL_VS} == 2'b11) begin   //SW turned off in the middle of capturing current frame - keep capturing until next frame
 		capture <= 1;
	end else if(~SW[9] && capture == 0 && {pre_vs,LUT_MIPI_PIXEL_VS} == 2'b11) begin   //SW off while no capturing in progress
 		capture <= 0;
 	end else if(~SW[9] && {pre_vs,LUT_MIPI_PIXEL_VS} == 2'b10) begin   //SW off at end of current frame - stop capturing
 		capture <= 0;
 	end else if(SW[9] && capture == 0 && {pre_vs,LUT_MIPI_PIXEL_VS} == 2'b11) begin   //SW on in middle of frame - don't capture until start of new frame
		capture <= 0;
	end else if(SW[9] && capture == 1 && {pre_vs,LUT_MIPI_PIXEL_VS} == 2'b11) begin   //capturing in progress - showing live video
		capture <= 1;
	end else if(SW[9] && {pre_vs,LUT_MIPI_PIXEL_VS} == 2'b01) begin   //SW on at start of frame - start capturing
		capture <= 1;
	end else if(SW[9] && {pre_vs,LUT_MIPI_PIXEL_VS} == 2'b10) begin   //SW turned on at end of current frame - wait until start of next frame before capturing
		capture <= 0;
	end else begin
 		capture <= SW[9];
 	end
 end

reg prev_capture;

always @(posedge CLOCK2_50) begin
	prev_capture <= capture;

	case({prev_capture, capture})
		2'b0: begin
			if(img_rd_done) begin   //curr img handled, reset img_captured
				img_captured <= 0;  
			end else begin
				img_captured <= img_captured;
			end

		end
		2'b01: img_captured <= 0;
		2'b10: img_captured <= 1;
		2'b11: img_captured <= 0;
	endcase
end


//checking for frame capture
// always @(*) begin
// 	LEDR[0] <= LUT_MIPI_PIXEL_VS;
// 	LEDR[3:2] <= {pre_vs,LUT_MIPI_PIXEL_VS};
// 	LEDR[5] <= capture;
// end


// FpsMonitor uFps( 
// 	   .clk50    ( CLOCK2_50 ),
// 	   .vs       ( LUT_MIPI_PIXEL_VS ),
	
// 	   .fps      (),
// 	   .hex_fps_h( HEX1 ),
// 	   .hex_fps_l( HEX0 )
// );


//--LED DISPLAY--
CLOCKMEM  ck1 ( .CLK(VGA_CLK )   ,.CLK_FREQ  (25000000  ) , . CK_1HZ (D8M_CK_HZ   )  )        ;//25MHZ
CLOCKMEM  ck2 ( .CLK(MIPI_REFCLK   )   ,.CLK_FREQ  (20000000   ) , . CK_1HZ (D8M_CK_HZ2  )  ) ;//20MHZ
CLOCKMEM  ck3 ( .CLK(MIPI_PIXEL_CLK_)   ,.CLK_FREQ  (25000000  ) , . CK_1HZ (D8M_CK_HZ3  )  )  ;//25MHZ


// assign LEDR = { D8M_CK_HZ ,D8M_CK_HZ2,D8M_CK_HZ3 ,5'h0,CAMERA_MIPI_RELAESE ,MIPI_BRIDGE_RELEASE  } ; 

endmodule
