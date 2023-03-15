module uart_wifi_transfer (
    input clk, input rst_n,
    input pix_rdy, input uart_rdy,
    input[23:0] pixel_data, 
    // master (SDRAM-facing)
    input logic master_waitrequest,
    output logic [31:0] master_address,
    output logic master_read, input logic [31:0] master_readdata, input logic master_readdatavalid,
    output logic master_write, output logic [31:0] master_writedata);

    logic [23:0] curr_pix_data;
    always @(posedge clk) begin
        if(!rst_n) begin

        end else begin
            case(state)

                GET_PIXEL: begin   //default state
                    if(pix_rdy && uart_rdy) begin
                        curr_pix_data <= pixel_data;
                        state <= XFER_PIXEL;
                    end
                end

                XFER_PIXEL: begin
                    if(!master_waitrequest ) begin

                    end
                end
            endcase
        end
    end
endmodule