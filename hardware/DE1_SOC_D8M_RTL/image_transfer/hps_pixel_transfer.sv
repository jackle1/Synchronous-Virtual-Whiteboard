module hps_pixel_transfer (
    input clk, input rst_n,
    input pix_rdy,  //pix_rdy = img_captured
    input[9:0] rd2_data, output reg vga2_req, //
    // output logic [23:0] pix_rgb_out, output logic [31:0] pix_rdy_out,

    output logic vga_clk,
    
    // slave signals (CPU-facing)
    output logic waitrequest,
    input logic [2:0] address,
    input logic read,
    output logic [31:0] readdata,
    input logic write,
    input logic [31:0] writedata
);
    //CPU read/write
    //addr 0 - next_pixel_rdy: readdata = 1 if pixel rdy, = 2 if done
    //addr 1 - ack: writedata = 1 if cpu rdy to receive
    //addr 2 - readdata = pixel data

    //CPU <--> Image Reader Handshake: 
    //CPU checks pixel_rdy (while - alternates b/w checking camera or touchscreen or wifi)
    // --> img captured, readdata = 1st pix, pixel_rdy = 1 (SET_RDY), slave waits for ack (WAIT_ACK)
    // --> CPU sees pixel_rdy = 1, set writedata ack = 1,
    // --> slave sees ack = 1, set pixel_rdy = 0 (WAIT_ACK)
    // --> CPU reads pix data frm addr 2, sets ack = 0 (means done w curr pixel), waits for pixel_rdy
    // --> slave sees ack = 0, set readdata = next pix, once done sets pixel_rdy = 1
    // --> if curr pixel = last, slave sets addr 0 = 2 for done

    // logic cpu_rdy;
    logic [23:0] pix_rgb_out;
    logic [31:0] pix_rdy_out;

    // wire vga2_req;
    wire [9:0] sdram_rd_data;

    //VGA controller 2
    wire VGA_BLANK_N2, VGA_HS2, VGA_SYNC_N2, VGA_VS2;
    wire [7:0] RED2, GREEN2, BLUE2;
    wire [12:0] VGA_H_CNT2, VGA_V_CNT2;
    wire [7:0] R_AUTO2, G_AUTO2, B_AUTO2;

    reg [10:0] rd_x_cont;
    reg [10:0] rd_y_cont;

    logic [31:0] prev_writedata;

    RAW2RGB_J rgb_converter2 (	
            .RST          ( VGA_VS2 ),
            .iDATA        ( rd2_data ),

            //-----------------------------------
            .VGA_CLK      ( vga_clk ),
            .READ_Request ( vga2_req ),
            .VGA_VS       ( VGA_VS2 ),	
            .VGA_HS       ( VGA_HS2 ) , 
                
            .oRed         ( RED2  ),
            .oGreen       ( GREEN2),
            .oBlue        ( BLUE2 ),
            .mX_Cont      ( rd_x_cont ),
            .mY_Cont      ( rd_y_cont )
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
            .iCLK       ( vga_clk ),
            .iRST_N     ( rst_n ),
            .H_Cont     ( VGA_H_CNT2 ),						
            .V_Cont     ( VGA_V_CNT2 )								
    );



    enum {WAIT_SET_RDY, WAIT_ACK, GET_NEXT_PIX, SEND_PIX} state;

    logic [3:0] out_state;

    logic [19:0] count;
    
    logic [9:0] wanted_x;
    logic [9:0] wanted_y;

    always @(posedge clk) begin
        if(!rst_n) begin
            pix_rdy_out <= 0;

            count <= 0;
            state <= WAIT_SET_RDY;
        end else begin
            case (state)

                WAIT_SET_RDY: begin
                    
                    count <= 0;

                    waitrequest <= 0;
                    // pix_rdy_out <= 0;
                    if(pix_rdy) begin 
                        pix_rdy_out <= 1;
                        // rd2_req <= 1;
                        prev_writedata <= 32'b1111;
                        state <= WAIT_ACK;
                    end
                end

                WAIT_ACK: begin
                    // rd2_req <= 1;
                    vga_clk <= 0;                   
                    
                    if(write && address == 3'd0 && prev_writedata !== writedata) begin
                        wanted_x <= writedata[19:10];
                        wanted_y <= writedata[9:0];
                        
                        prev_writedata <= writedata;
                        
                        // vga_clk <= 1;  
                        state <= GET_NEXT_PIX;  
                    end

                    
                end

                GET_NEXT_PIX: begin
                    vga_clk <= 1;
                    waitrequest <= 1;
                    state <= SEND_PIX;
                end

                SEND_PIX: begin

                    vga_clk <= 0;

                    if(rd_x_cont == wanted_x && rd_y_cont == wanted_y) begin
                        count <= count + 1;
                        pix_rgb_out <= {R_AUTO2[7:0], G_AUTO2[7:0], B_AUTO2[7:0]};
                        waitrequest <= 0;
                        state <= WAIT_ACK;

                    end else begin
                        waitrequest <= 1;
                        state <= GET_NEXT_PIX;

                    end
                    
                end

                // DONE: begin

                //     pix_rdy_out <= 32'd2;
                //     // state <= WAIT_SET_RDY;
                // end

                default: begin
                    pix_rdy_out <= 0;
                    state <= WAIT_SET_RDY;
                end
            endcase
        end
    end

    //readdata output
    always @(*) begin
        if(read) begin
            case (address)
                3'd0: readdata <= pix_rdy_out;
                3'd1: readdata <= {8'b0, pix_rgb_out};
                // 3'd1: readdata <= 32'h1234abcd;
            endcase
        end
    end

endmodule
