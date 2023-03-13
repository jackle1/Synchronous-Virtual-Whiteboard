# References

## Wifi Adapter
* wifi/test/test_server.js - https://esp8266-shop.com/iot/websocket-connection-between-esp8266-and-node-js-server/
* wifi/init.lua - https://nodemcu.readthedocs.io/en/release/upload/
* ESPlorer & PyFlasher - https://nodemcu.readthedocs.io/en/release/getting-started/

## D8M Camera
* Terasic D8M GPIO Camera Resources: https://www.terasic.com.tw/cgi-bin/page/archive.pl?Language=English&CategoryNo=68&No=1011&PartNo=4#contents

## HPS System References
* HPS Top Level File Example - https://people.ece.cornell.edu/land/courses/ece5760/DE1_SOC/HPS_FPGA/pio_test/ghrd_top.v
* SoC-FPGA Design Guide - https://people.ece.cornell.edu/land/courses/ece5760/DE1_SOC/SoC-FPGA%20Design%20Guide_EPFL.pdf

## VGA Picture Converter
* BMP File Format Guide - https://www.file-recovery.com/bmp-signature-format.htm
* Sampling Algorithm To Convert From RGB888 to RGB565 - https://www.barth-dev.de/about-rgb565-and-how-to-convert-into-it/

# Notes
## D8M Camera
* Transferring pixels from camera (sdram) to cpu using nios working
  * Pixel transferred from: sdram read fifo 2 --> img_cpu_reader --> hps_img_transfer --> avalon_slave --> cpu
* Confirmed sdram contents are being transferred to cpu thru avalon-slave and custom handshake. Further steps to verify if pixels from image were correctly written into required address in sdram 
* Image Capture changed. Now, assert SW9 to see video feed. Deassert SW9 to capture image. Press KEY3 to send image to cpu
### To Do
* Camera image (at least when viewing thru VGA monitor) gets pixelated and rgb palate seems to have decreased when top file (DE1_SOC_D8M_RTL.v) instantiates qsys component (camera_module) --> check if quality of image stored in sdram is affected. If so, find solution or workaround, otherwise can leave as is as long as captured image quality unaffected
* If possible, combine module retrieving pixel from sdram (img_cpu_reader) and module implementing handshake between cpu and img_cpu_reader (hps_img_transfer) into 1 state machine
