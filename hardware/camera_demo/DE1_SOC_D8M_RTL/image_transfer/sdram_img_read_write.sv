`include  "../V_Sdram_Control/Sdram_Params.h"

module img_cpu_reader (
    input clk, input rst_n,   //clk == VGA_CLK  
    // input img_convert_done,   //== 1 when RAW2RGB for whole frame is done
    
    // output rd_req,   //rd_req == READ_Request for sdram read 2 fifo
    // output rd_fifo_clear,   //== read 2 fifo aclr
    // // input [7:0] RGB_Red, input [7:0] RGB_Green, input [7:0] RGB_Blue,
    // input [15:0] sdram_data,
    // input VGA_VS, input VGA_HS,
    // input raw2rgb_ready,  //from raw2rgb, first pixel ready


    input img_captured,
    input ack,
    output logic pixel_rdy,
    output logic img_done,
    output [7:0] RED2,
    output [7:0] GREEN2,
    output [7:0] BLUE2,
    input [9:0] rd2_data,
    output logic rd2_req,
    input [`ASIZE-1:0] start_addr,
    input [`ASIZE-1:0] max_addr,
    input [10:0] rd_len,
    output [3:0] curr_state,
    input send_img,
    output [8:0] loop_count
);
    wire vga2_req;
    wire [9:0] sdram_rd_data;

    //VGA controller 2
    wire VGA_BLANK_N2, VGA_HS2, VGA_SYNC_N2, VGA_VS2;
    // wire [7:0] RED2, GREEN2, BLUE2;
    wire [12:0] VGA_H_CNT2, VGA_V_CNT2;
    wire [7:0] R_AUTO2, G_AUTO2, B_AUTO2;

    RAW2RGB_J rgb_converter2 (	
            .RST          ( VGA_VS2 ),
            .iDATA        ( sdram_rd_data ),

            //-----------------------------------
            .VGA_CLK      ( clk ),
            .READ_Request ( vga2_req ),
            .VGA_VS       ( VGA_VS2 ),	
            .VGA_HS       ( VGA_HS2 ) , 
                
            .oRed         ( RED2  ),
            .oGreen       ( GREEN2),
            .oBlue        ( BLUE2 )
		);

    VGA_Controller vga2 (
            .oRequest( vga2_req ),
            .iRed    ( RED2    ),
            .iGreen  ( GREEN2  ),
            .iBlue   ( BLUE2   ),
            
            //	VGA Side
            .oVGA_R  ( R_AUTO2[7:0] ),
            .oVGA_G  ( G_AUTO2[7:0] ),
            .oVGA_B  ( B_AUTO2[7:0] ),
            .oVGA_H_SYNC( VGA_HS2 ),
            .oVGA_V_SYNC( VGA_VS2 ),
            .oVGA_SYNC  ( VGA_SYNC_N2 ),
            .oVGA_BLANK ( VGA_BLANK_N2 ),
            //	Control Signal
            .iCLK       ( clk ),
            .iRST_N     ( rst_n ),
            .H_Cont     ( VGA_H_CNT2 ),						
            .V_Cont     ( VGA_V_CNT2 )								
    );


    logic next_rd_req;
    logic [`ASIZE-1:0] curr_addr;
    enum {IDLE, WAIT_ACK, SDRAM_READ1, SDRAM_READ2, WAIT_RDY_ACK, DONE} state;

    // assign pixel_rdy = img_captured;

    always @(posedge clk) begin
        if (!rst_n) begin
            img_done <= 0;
            pixel_rdy <= 0;

            state <= IDLE;
        end else begin
            case(state)
                IDLE: begin
                    curr_state <= 4'd0;

                    pixel_rdy <= 0;

                    // img_done <= 0;

                    loop_count <= 0;
                    
                    if(img_captured & ~send_img) begin
                        curr_state <= 4'd6;
                        curr_addr <= start_addr;
                        state <= WAIT_ACK;
                    end else begin
                        state <= IDLE;
                    end
                end

                WAIT_ACK: begin
                    curr_state <= 4'd8;
                    pixel_rdy <= 1;
                    img_done <= 0;
                    if(ack && curr_addr < max_addr-rd_len) begin
                        next_rd_req <= 1;

                        curr_state <= 4'd1;
                        
                        state <= SDRAM_READ1;
                    end else if (curr_addr >= max_addr-rd_len) begin
                        next_rd_req <= 0;
                        state <= DONE;
                    end else begin
                        next_rd_req <= 0;
                        state <= WAIT_ACK;
                    end

                end

                SDRAM_READ1: begin
                    curr_state <= 4'd2;

                    next_rd_req <= 0;
                    state <= SDRAM_READ2;
                end

                SDRAM_READ2: begin   //is this extra clk needed for raw2rgb?
                    if(loop_count < 9'b1_1111_1111);
                        loop_count <= loop_count+1;
                    
                    curr_state <= 4'd3;
                    
                    sdram_rd_data <= rd2_data;
                    curr_addr <= curr_addr+rd_len;
                    state <= WAIT_RDY_ACK;
                    // state <= SDRAM_READ2;
                end

                WAIT_RDY_ACK: begin
                    curr_state <= 4'd4;
                    if (!ack) begin
                        state <= WAIT_ACK;
                        // state <= DONE;
                    end else begin
                        state <= WAIT_RDY_ACK;
                    end
                end

                DONE: begin
                    curr_state <= 4'd5;

                    img_done <= 1;
                    pixel_rdy <= 0;

                    if(!img_captured) begin
                        state <= IDLE;
                    end
                    
                end

                default: state <= IDLE;
            endcase
        end 
    end

    always @(negedge clk) begin
        rd2_req <= next_rd_req;
    end

endmodule