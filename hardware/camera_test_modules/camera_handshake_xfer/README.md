Camera Implementation Using Handshake Between HPS and DE1_SOC_D8M_RTL

- Custom Module: DE1_SOC_D8M_RTL/image_transfer/hps_pixel_transfer.sv
- Uses 2nd RAW2RGB and rd2_fifo to read MIPI pixels from sdram and convert to rgb --> state machine generates rd_req
- Transfers 16b rgb pixels over hps_lw_axi
- Handshake waits until writedata value from HPS control has changed before sending next pixel

HPS Control
- Custom module: hps_control/main_camera_test.c
- Reads rgb pixels from sdram, writes to file (to send to cloud) and to vga_base to display on whiteboard

testHPS:
- Updated cpu.qsys with hps_pixel_transfer module