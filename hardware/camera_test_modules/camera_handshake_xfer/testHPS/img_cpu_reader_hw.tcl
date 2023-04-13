# TCL File Generated by Component Editor 18.1
# Tue Apr 11 06:07:19 PDT 2023
# DO NOT MODIFY


# 
# img_cpu_reader "img_cpu_reader" v1.0
#  2023.04.11.06:07:19
# 
# 

# 
# request TCL package from ACDS 16.1
# 
package require -exact qsys 16.1


# 
# module img_cpu_reader
# 
set_module_property DESCRIPTION ""
set_module_property NAME img_cpu_reader
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property AUTHOR ""
set_module_property DISPLAY_NAME img_cpu_reader
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL hps_pixel_transfer
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file hps_pixel_transfer.sv SYSTEM_VERILOG PATH ../DE1_SOC_D8M_RTL/image_transfer/hps_pixel_transfer.sv TOP_LEVEL_FILE


# 
# parameters
# 


# 
# display items
# 


# 
# connection point clock
# 
add_interface clock clock end
set_interface_property clock clockRate 0
set_interface_property clock ENABLED true
set_interface_property clock EXPORT_OF ""
set_interface_property clock PORT_NAME_MAP ""
set_interface_property clock CMSIS_SVD_VARIABLES ""
set_interface_property clock SVD_ADDRESS_GROUP ""

add_interface_port clock clk clk Input 1


# 
# connection point avalon_slave_0
# 
add_interface avalon_slave_0 avalon end
set_interface_property avalon_slave_0 addressUnits WORDS
set_interface_property avalon_slave_0 associatedClock clock
set_interface_property avalon_slave_0 associatedReset reset_sink
set_interface_property avalon_slave_0 bitsPerSymbol 8
set_interface_property avalon_slave_0 burstOnBurstBoundariesOnly false
set_interface_property avalon_slave_0 burstcountUnits WORDS
set_interface_property avalon_slave_0 explicitAddressSpan 0
set_interface_property avalon_slave_0 holdTime 0
set_interface_property avalon_slave_0 linewrapBursts false
set_interface_property avalon_slave_0 maximumPendingReadTransactions 0
set_interface_property avalon_slave_0 maximumPendingWriteTransactions 0
set_interface_property avalon_slave_0 readLatency 0
set_interface_property avalon_slave_0 readWaitTime 1
set_interface_property avalon_slave_0 setupTime 0
set_interface_property avalon_slave_0 timingUnits Cycles
set_interface_property avalon_slave_0 writeWaitTime 0
set_interface_property avalon_slave_0 ENABLED true
set_interface_property avalon_slave_0 EXPORT_OF ""
set_interface_property avalon_slave_0 PORT_NAME_MAP ""
set_interface_property avalon_slave_0 CMSIS_SVD_VARIABLES ""
set_interface_property avalon_slave_0 SVD_ADDRESS_GROUP ""

add_interface_port avalon_slave_0 waitrequest waitrequest Output 1
add_interface_port avalon_slave_0 address address Input 3
add_interface_port avalon_slave_0 read read Input 1
add_interface_port avalon_slave_0 readdata readdata Output 32
add_interface_port avalon_slave_0 write write Input 1
add_interface_port avalon_slave_0 writedata writedata Input 32
set_interface_assignment avalon_slave_0 embeddedsw.configuration.isFlash 0
set_interface_assignment avalon_slave_0 embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment avalon_slave_0 embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment avalon_slave_0 embeddedsw.configuration.isPrintableDevice 0


# 
# connection point reset_sink
# 
add_interface reset_sink reset end
set_interface_property reset_sink associatedClock clock
set_interface_property reset_sink synchronousEdges DEASSERT
set_interface_property reset_sink ENABLED true
set_interface_property reset_sink EXPORT_OF ""
set_interface_property reset_sink PORT_NAME_MAP ""
set_interface_property reset_sink CMSIS_SVD_VARIABLES ""
set_interface_property reset_sink SVD_ADDRESS_GROUP ""

add_interface_port reset_sink rst_n reset_n Input 1


# 
# connection point pix_rdy
# 
add_interface pix_rdy conduit end
set_interface_property pix_rdy associatedClock clock
set_interface_property pix_rdy associatedReset ""
set_interface_property pix_rdy ENABLED true
set_interface_property pix_rdy EXPORT_OF ""
set_interface_property pix_rdy PORT_NAME_MAP ""
set_interface_property pix_rdy CMSIS_SVD_VARIABLES ""
set_interface_property pix_rdy SVD_ADDRESS_GROUP ""

add_interface_port pix_rdy pix_rdy pix_rdy Input 1


# 
# connection point rd2_data
# 
add_interface rd2_data conduit end
set_interface_property rd2_data associatedClock clock
set_interface_property rd2_data associatedReset ""
set_interface_property rd2_data ENABLED true
set_interface_property rd2_data EXPORT_OF ""
set_interface_property rd2_data PORT_NAME_MAP ""
set_interface_property rd2_data CMSIS_SVD_VARIABLES ""
set_interface_property rd2_data SVD_ADDRESS_GROUP ""

add_interface_port rd2_data rd2_data rd2_data Input 10


# 
# connection point vga_clk
# 
add_interface vga_clk conduit end
set_interface_property vga_clk associatedClock clock
set_interface_property vga_clk associatedReset reset_sink
set_interface_property vga_clk ENABLED true
set_interface_property vga_clk EXPORT_OF ""
set_interface_property vga_clk PORT_NAME_MAP ""
set_interface_property vga_clk CMSIS_SVD_VARIABLES ""
set_interface_property vga_clk SVD_ADDRESS_GROUP ""

add_interface_port vga_clk vga_clk vga_clk Output 1


# 
# connection point vga2_req
# 
add_interface vga2_req conduit end
set_interface_property vga2_req associatedClock clock
set_interface_property vga2_req associatedReset reset_sink
set_interface_property vga2_req ENABLED true
set_interface_property vga2_req EXPORT_OF ""
set_interface_property vga2_req PORT_NAME_MAP ""
set_interface_property vga2_req CMSIS_SVD_VARIABLES ""
set_interface_property vga2_req SVD_ADDRESS_GROUP ""

add_interface_port vga2_req vga2_req vga2_req Output 1
