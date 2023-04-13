module hps_pixel_transfer (
    input clk, input rst_n,
    input pix_rdy, input img_done,
    input[15:0] pixel_data, output get_next_pix,
    output logic [3:0] out_state, output logic cpu_rdy,
    output logic [23:0] pix_rgb_out, output logic [31:0] pix_rdy_out,
    
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
    // logic [23:0] pix_rgb_out;
    // logic [31:0] pix_rdy_out;

    enum {WAIT_SET_RDY, WAIT_ACK, SEND_PIX, GET_NEXT_PIX, DONE} state;

    wire vga_clk;

    VIDEO_PLL pll_ref1(
	                   .inclk0 ( clk ),
	                   .areset ( ~rst_n ),
	                   .c0( vga_clk )        //25 Mhz	
    );	

    // logic [3:0] out_state;

    logic [31:0] out_data;

    always @(posedge vga_clk) begin
        if(!rst_n) begin
            pix_rdy_out <= 0;
            out_data <= 32'b0;
            // state <= WAIT_SET_RDY;
            state <= WAIT_ACK;
        end else begin
            case (state)

                WAIT_SET_RDY: begin

                    out_state <= 4'd0;
                    get_next_pix <= 0;
                    if(pix_rdy) begin
                        pix_rdy_out <= 0;
                        state <= WAIT_ACK;
                    end
                end

                // WAIT_ACK: begin
                //     cpu_rdy <= 0;
                //     out_state <= 4'd1;
                //     if(write && address == 3'd1) begin
                //         cpu_rdy <= writedata;    
                //     end

                //     if(cpu_rdy)
                //         state <= SEND_PIX;
                // end

                WAIT_ACK: begin
                    cpu_rdy <= 0;
                    out_state <= 4'd1;
                    pix_rdy_out <= 0;
                    prev_writedata <= (writedata == 0) ? 32'hffff_ffff : writedata;
                    
                    if(write && pix_rdy && (prev_writedata !== writedata)) begin
                        out_data <= writedata;
                        get_next_pix <= 1;  //== rd2_req
                        state <= SEND_PIX;  
                    end

                end

                // SEND_PIX: begin
                //     out_state <= 4'd2;
                //     pix_rgb_out <= pixel_data;
                //     state <= GET_NEXT_PIX;
                // end

                SEND_PIX: begin
                    pix_rgb_out <= pixel_data;
                    get_next_pix <= 0;
                    pix_rdy_out <= 1;
                    state <= WAIT_ACK;
                end

                // GET_NEXT_PIX: begin
                //     out_state <= 4'd3;

                //     if(write && address == 3'd1) begin
                //         cpu_rdy <= writedata;   //cpu_rdy deasserted if done w curr pix
                //     end

                //     if(img_done && !pix_rdy) begin
                //         get_next_pix <= 0;
                //         state <= DONE;
                //     end else if(!cpu_rdy && !img_done) begin
                //         get_next_pix <= 1;
                //         state <= WAIT_SET_RDY;
                //     end
                // end

                // DONE: begin
                //     out_state <= 4'd5;

                //     pix_rdy_out <= 32'd2;
                //     state <= WAIT_SET_RDY;
                // end

                default: begin
                    pix_rdy_out <= 0;
                    get_next_pix <= 0;
                    state <= WAIT_SET_RDY;
                end
            endcase
        end
    end

    //readdata output
    always @(*) begin
        if(read) begin
            case (address)
                3'd0: readdata <= pix_rgb_out;
                3'd1: readdata <= pix_rdy_out;
                default: readdata <= 32'b0;
            endcase
        end
    end

endmodule