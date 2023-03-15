module hps_pixel_transfer (
    input clk, input rst_n,
    input pix_rdy, input img_done,
    input[23:0] pixel_data, output get_next_pix,
    output logic [3:0] out_state, output logic cpu_rdy,
    output logic [23:0] pix_rgb_out, output logic [31:0] pix_rdy_out,
    
    // slave signals (CPU-facing)
    output logic waitrequest,
    input logic [2:0] address,
    input logic read,
    output logic [31:0] readdata,
    input logic write,
    input logic [31:0] writedata,
    output logic [3:0] out_state;
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

    enum {WAIT_SET_RDY, WAIT_ACK, SEND_PIX, GET_NEXT_PIX, DONE} state;

    // logic [3:0] out_state;


    always @(posedge clk) begin
        if(!rst_n) begin
            pix_rdy_out <= 0;
            state <= WAIT_SET_RDY;
        end else begin
            case (state)
                // IDLE: begin
                //     pix_rdy_out <= 0;
                //     if(pix_rdy && !img_done) begin
                //         state <= WAIT_SET_RDY;
                //     end
                // end

                WAIT_SET_RDY: begin

                    out_state <= 4'd0;
                    // pix_rdy_out <= 0;
                    if(pix_rdy) begin   //if 
                        pix_rdy_out <= 1;
                        state <= WAIT_ACK;
                    end
                end

                WAIT_ACK: begin
                    cpu_rdy <= 0;
                    out_state <= 4'd1;
                    if(address == 3'd1) begin
                        cpu_rdy <= writedata;    
                    end

                    if(cpu_rdy)
                        state <= SEND_PIX;
                end

                SEND_PIX: begin
                    out_state <= 4'd2;
                    pix_rgb_out <= pixel_data;
                    state <= GET_NEXT_PIX;
                end

                GET_NEXT_PIX: begin
                    out_state <= 4'd3;

                    if(address == 3'd1) begin
                        cpu_rdy <= writedata;   //cpu_rdy deasserted if done w curr pix
                    end

                    if(img_done && !pix_rdy) begin
                        get_next_pix <= 0;
                        state <= DONE;
                    end else if(!cpu_rdy && !img_done) begin
                        get_next_pix <= 1;
                        state <= WAIT_SET_RDY;
                    end
                end

                DONE: begin
                    out_state <= 4'd5;

                    pix_rdy_out <= 32'd2;
                    state <= WAIT_SET_RDY;
                end

                default: begin
                    pix_rdy_out <= 0;
                    state <= WAIT_SET_RDY;
                end
            endcase
        end
    end

    //readdata output
    always @(*) begin
        case (address)
            3'd0: readdata <= pix_rdy_out;
            3'd2: readdata <= pix_rgb_out;
        endcase
    end

endmodule