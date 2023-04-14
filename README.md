# L2B - Team #25 - Synchronous Virtual Whiteboard

## Teammates
- Ranbir Sharma (83481523)
- Jack Le (64004500)
- Jane Wu (47445887)
- Jacky Le (80260029)

## Contributions
- Jacky
  - AR1100 Touchscreen Controller 
  - ESP8266 Wifi Module
  - HPS Software Control
  - D8M Camera Integration

- Jack
  - Web Pages and Components
  - Web App to Cloud WebSocket Connection and HTTP Requests
  - Page Routing
  
- Jane
  - D8M Camera Integration
  - D8M Camera Test Modules

- Ranbir Sharma 
  - Implementing and maintaining AWS services like dyanmoDB, API gateway and S3 bucket
  - Reaching and sending pixels to the active member on a particular room
  - maintaining and saving pixels to cloud S3

## References

### Quartus
* Provided many hardware components such as the UART module, PIO's, VGA Controllers, etc.

### Intel Video & Image Processing (VIP) Suite
* Provided Qsys IPs: Frame Buffer and Clock Video Output

### Terasic
* Provided Qsys IPs from DE1_SOC_D8M_VIP code demo: Terasic Camera and Terasic Auto-Focus IPs

### Wifi Adapter
* wifi/test/test_server.js - https://esp8266-shop.com/iot/websocket-connection-between-esp8266-and-node-js-server/
* wifi/init.lua - https://nodemcu.readthedocs.io/en/release/upload/
* ESPlorer & PyFlasher - https://nodemcu.readthedocs.io/en/release/getting-started/

### D8M Camera
* Terasic D8M GPIO Camera Resources: https://www.terasic.com.tw/cgi-bin/page/archive.pl?Language=English&CategoryNo=68&No=1011&PartNo=4#contents

### HPS System References
* HPS Top Level File Example - https://people.ece.cornell.edu/land/courses/ece5760/DE1_SOC/HPS_FPGA/pio_test/ghrd_top.v
* SoC-FPGA Design Guide - https://people.ece.cornell.edu/land/courses/ece5760/DE1_SOC/SoC-FPGA%20Design%20Guide_EPFL.pdf

### VGA Picture Converter
* BMP File Format Guide - https://www.file-recovery.com/bmp-signature-format.htm
* Sampling Algorithm To Convert From RGB888 to RGB565 - https://www.barth-dev.de/about-rgb565-and-how-to-convert-into-it/
* Background Picture: https://pixabay.com/illustrations/acrylic-paint-canvas-background-2198458/ 

### Libcurl
* HTTP Requests & sockets - https://curl.se/libcurl/
* Sockets Example - https://curl.se/libcurl/c/sendrecv.html 

### AWS Documention 
* DynamoDB - https://docs.aws.amazon.com/dynamodb/?icmpid=docs_homepage_featuredsvcs
* S3 - https://docs.aws.amazon.com/s3/?icmpid=docs_homepage_featuredsvcs
* API Gateway - https://docs.aws.amazon.com/apigateway/?icmpid=docs_homepage_serverless
* AWS Lambda - https://docs.aws.amazon.com/lambda/?icmpid=docs_homepage_serverless
