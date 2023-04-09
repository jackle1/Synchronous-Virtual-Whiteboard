	component Qsys is
		port (
			alt_vip_itc_0_clocked_video_vid_clk       : in    std_logic                     := 'X';             -- vid_clk
			alt_vip_itc_0_clocked_video_vid_data      : out   std_logic_vector(23 downto 0);                    -- vid_data
			alt_vip_itc_0_clocked_video_underflow     : out   std_logic;                                        -- underflow
			alt_vip_itc_0_clocked_video_vid_datavalid : out   std_logic;                                        -- vid_datavalid
			alt_vip_itc_0_clocked_video_vid_v_sync    : out   std_logic;                                        -- vid_v_sync
			alt_vip_itc_0_clocked_video_vid_h_sync    : out   std_logic;                                        -- vid_h_sync
			alt_vip_itc_0_clocked_video_vid_f         : out   std_logic;                                        -- vid_f
			alt_vip_itc_0_clocked_video_vid_h         : out   std_logic;                                        -- vid_h
			alt_vip_itc_0_clocked_video_vid_v         : out   std_logic;                                        -- vid_v
			clk_clk                                   : in    std_logic                     := 'X';             -- clk
			clk_sdram_clk                             : out   std_logic;                                        -- clk
			clk_vga_clk                               : out   std_logic;                                        -- clk
			d8m_xclkin_clk                            : out   std_logic;                                        -- clk
			hps_io_hps_io_emac1_inst_TX_CLK           : out   std_logic;                                        -- hps_io_emac1_inst_TX_CLK
			hps_io_hps_io_emac1_inst_TXD0             : out   std_logic;                                        -- hps_io_emac1_inst_TXD0
			hps_io_hps_io_emac1_inst_TXD1             : out   std_logic;                                        -- hps_io_emac1_inst_TXD1
			hps_io_hps_io_emac1_inst_TXD2             : out   std_logic;                                        -- hps_io_emac1_inst_TXD2
			hps_io_hps_io_emac1_inst_TXD3             : out   std_logic;                                        -- hps_io_emac1_inst_TXD3
			hps_io_hps_io_emac1_inst_RXD0             : in    std_logic                     := 'X';             -- hps_io_emac1_inst_RXD0
			hps_io_hps_io_emac1_inst_MDIO             : inout std_logic                     := 'X';             -- hps_io_emac1_inst_MDIO
			hps_io_hps_io_emac1_inst_MDC              : out   std_logic;                                        -- hps_io_emac1_inst_MDC
			hps_io_hps_io_emac1_inst_RX_CTL           : in    std_logic                     := 'X';             -- hps_io_emac1_inst_RX_CTL
			hps_io_hps_io_emac1_inst_TX_CTL           : out   std_logic;                                        -- hps_io_emac1_inst_TX_CTL
			hps_io_hps_io_emac1_inst_RX_CLK           : in    std_logic                     := 'X';             -- hps_io_emac1_inst_RX_CLK
			hps_io_hps_io_emac1_inst_RXD1             : in    std_logic                     := 'X';             -- hps_io_emac1_inst_RXD1
			hps_io_hps_io_emac1_inst_RXD2             : in    std_logic                     := 'X';             -- hps_io_emac1_inst_RXD2
			hps_io_hps_io_emac1_inst_RXD3             : in    std_logic                     := 'X';             -- hps_io_emac1_inst_RXD3
			hps_io_hps_io_qspi_inst_IO0               : inout std_logic                     := 'X';             -- hps_io_qspi_inst_IO0
			hps_io_hps_io_qspi_inst_IO1               : inout std_logic                     := 'X';             -- hps_io_qspi_inst_IO1
			hps_io_hps_io_qspi_inst_IO2               : inout std_logic                     := 'X';             -- hps_io_qspi_inst_IO2
			hps_io_hps_io_qspi_inst_IO3               : inout std_logic                     := 'X';             -- hps_io_qspi_inst_IO3
			hps_io_hps_io_qspi_inst_SS0               : out   std_logic;                                        -- hps_io_qspi_inst_SS0
			hps_io_hps_io_qspi_inst_CLK               : out   std_logic;                                        -- hps_io_qspi_inst_CLK
			hps_io_hps_io_sdio_inst_CMD               : inout std_logic                     := 'X';             -- hps_io_sdio_inst_CMD
			hps_io_hps_io_sdio_inst_D0                : inout std_logic                     := 'X';             -- hps_io_sdio_inst_D0
			hps_io_hps_io_sdio_inst_D1                : inout std_logic                     := 'X';             -- hps_io_sdio_inst_D1
			hps_io_hps_io_sdio_inst_CLK               : out   std_logic;                                        -- hps_io_sdio_inst_CLK
			hps_io_hps_io_sdio_inst_D2                : inout std_logic                     := 'X';             -- hps_io_sdio_inst_D2
			hps_io_hps_io_sdio_inst_D3                : inout std_logic                     := 'X';             -- hps_io_sdio_inst_D3
			hps_io_hps_io_usb1_inst_D0                : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D0
			hps_io_hps_io_usb1_inst_D1                : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D1
			hps_io_hps_io_usb1_inst_D2                : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D2
			hps_io_hps_io_usb1_inst_D3                : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D3
			hps_io_hps_io_usb1_inst_D4                : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D4
			hps_io_hps_io_usb1_inst_D5                : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D5
			hps_io_hps_io_usb1_inst_D6                : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D6
			hps_io_hps_io_usb1_inst_D7                : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D7
			hps_io_hps_io_usb1_inst_CLK               : in    std_logic                     := 'X';             -- hps_io_usb1_inst_CLK
			hps_io_hps_io_usb1_inst_STP               : out   std_logic;                                        -- hps_io_usb1_inst_STP
			hps_io_hps_io_usb1_inst_DIR               : in    std_logic                     := 'X';             -- hps_io_usb1_inst_DIR
			hps_io_hps_io_usb1_inst_NXT               : in    std_logic                     := 'X';             -- hps_io_usb1_inst_NXT
			hps_io_hps_io_spim1_inst_CLK              : out   std_logic;                                        -- hps_io_spim1_inst_CLK
			hps_io_hps_io_spim1_inst_MOSI             : out   std_logic;                                        -- hps_io_spim1_inst_MOSI
			hps_io_hps_io_spim1_inst_MISO             : in    std_logic                     := 'X';             -- hps_io_spim1_inst_MISO
			hps_io_hps_io_spim1_inst_SS0              : out   std_logic;                                        -- hps_io_spim1_inst_SS0
			hps_io_hps_io_uart0_inst_RX               : in    std_logic                     := 'X';             -- hps_io_uart0_inst_RX
			hps_io_hps_io_uart0_inst_TX               : out   std_logic;                                        -- hps_io_uart0_inst_TX
			hps_io_hps_io_i2c0_inst_SDA               : inout std_logic                     := 'X';             -- hps_io_i2c0_inst_SDA
			hps_io_hps_io_i2c0_inst_SCL               : inout std_logic                     := 'X';             -- hps_io_i2c0_inst_SCL
			hps_io_hps_io_i2c1_inst_SDA               : inout std_logic                     := 'X';             -- hps_io_i2c1_inst_SDA
			hps_io_hps_io_i2c1_inst_SCL               : inout std_logic                     := 'X';             -- hps_io_i2c1_inst_SCL
			hps_io_hps_io_gpio_inst_GPIO09            : inout std_logic                     := 'X';             -- hps_io_gpio_inst_GPIO09
			hps_io_hps_io_gpio_inst_GPIO35            : inout std_logic                     := 'X';             -- hps_io_gpio_inst_GPIO35
			hps_io_hps_io_gpio_inst_GPIO40            : inout std_logic                     := 'X';             -- hps_io_gpio_inst_GPIO40
			hps_io_hps_io_gpio_inst_GPIO41            : inout std_logic                     := 'X';             -- hps_io_gpio_inst_GPIO41
			hps_io_hps_io_gpio_inst_GPIO48            : inout std_logic                     := 'X';             -- hps_io_gpio_inst_GPIO48
			hps_io_hps_io_gpio_inst_GPIO53            : inout std_logic                     := 'X';             -- hps_io_gpio_inst_GPIO53
			hps_io_hps_io_gpio_inst_GPIO54            : inout std_logic                     := 'X';             -- hps_io_gpio_inst_GPIO54
			hps_io_hps_io_gpio_inst_GPIO61            : inout std_logic                     := 'X';             -- hps_io_gpio_inst_GPIO61
			i2c_opencores_camera_export_scl_pad_io    : inout std_logic                     := 'X';             -- scl_pad_io
			i2c_opencores_camera_export_sda_pad_io    : inout std_logic                     := 'X';             -- sda_pad_io
			i2c_opencores_mipi_export_scl_pad_io      : inout std_logic                     := 'X';             -- scl_pad_io
			i2c_opencores_mipi_export_sda_pad_io      : inout std_logic                     := 'X';             -- sda_pad_io
			key_external_connection_export            : in    std_logic_vector(3 downto 0)  := (others => 'X'); -- export
			led_external_connection_export            : out   std_logic_vector(9 downto 0);                     -- export
			memory_mem_a                              : out   std_logic_vector(14 downto 0);                    -- mem_a
			memory_mem_ba                             : out   std_logic_vector(2 downto 0);                     -- mem_ba
			memory_mem_ck                             : out   std_logic;                                        -- mem_ck
			memory_mem_ck_n                           : out   std_logic;                                        -- mem_ck_n
			memory_mem_cke                            : out   std_logic;                                        -- mem_cke
			memory_mem_cs_n                           : out   std_logic;                                        -- mem_cs_n
			memory_mem_ras_n                          : out   std_logic;                                        -- mem_ras_n
			memory_mem_cas_n                          : out   std_logic;                                        -- mem_cas_n
			memory_mem_we_n                           : out   std_logic;                                        -- mem_we_n
			memory_mem_reset_n                        : out   std_logic;                                        -- mem_reset_n
			memory_mem_dq                             : inout std_logic_vector(31 downto 0) := (others => 'X'); -- mem_dq
			memory_mem_dqs                            : inout std_logic_vector(3 downto 0)  := (others => 'X'); -- mem_dqs
			memory_mem_dqs_n                          : inout std_logic_vector(3 downto 0)  := (others => 'X'); -- mem_dqs_n
			memory_mem_odt                            : out   std_logic;                                        -- mem_odt
			memory_mem_dm                             : out   std_logic_vector(3 downto 0);                     -- mem_dm
			memory_oct_rzqin                          : in    std_logic                     := 'X';             -- oct_rzqin
			mipi_pwdn_n_external_connection_export    : out   std_logic;                                        -- export
			mipi_reset_n_external_connection_export   : out   std_logic;                                        -- export
			reset_reset_n                             : in    std_logic                     := 'X';             -- reset_n
			sdram_wire_addr                           : out   std_logic_vector(12 downto 0);                    -- addr
			sdram_wire_ba                             : out   std_logic_vector(1 downto 0);                     -- ba
			sdram_wire_cas_n                          : out   std_logic;                                        -- cas_n
			sdram_wire_cke                            : out   std_logic;                                        -- cke
			sdram_wire_cs_n                           : out   std_logic;                                        -- cs_n
			sdram_wire_dq                             : inout std_logic_vector(15 downto 0) := (others => 'X'); -- dq
			sdram_wire_dqm                            : out   std_logic_vector(1 downto 0);                     -- dqm
			sdram_wire_ras_n                          : out   std_logic;                                        -- ras_n
			sdram_wire_we_n                           : out   std_logic;                                        -- we_n
			sw_external_connection_export             : in    std_logic_vector(9 downto 0)  := (others => 'X'); -- export
			terasic_auto_focus_0_conduit_vcm_i2c_sda  : inout std_logic                     := 'X';             -- vcm_i2c_sda
			terasic_auto_focus_0_conduit_clk50        : in    std_logic                     := 'X';             -- clk50
			terasic_auto_focus_0_conduit_vcm_i2c_scl  : inout std_logic                     := 'X';             -- vcm_i2c_scl
			terasic_camera_0_conduit_end_D            : in    std_logic_vector(11 downto 0) := (others => 'X'); -- D
			terasic_camera_0_conduit_end_FVAL         : in    std_logic                     := 'X';             -- FVAL
			terasic_camera_0_conduit_end_LVAL         : in    std_logic                     := 'X';             -- LVAL
			terasic_camera_0_conduit_end_PIXCLK       : in    std_logic                     := 'X';             -- PIXCLK
			touch_uart_RXD                            : in    std_logic                     := 'X';             -- RXD
			touch_uart_TXD                            : out   std_logic;                                        -- TXD
			hexes_export                              : out   std_logic_vector(27 downto 0)                     -- export
		);
	end component Qsys;

	u0 : component Qsys
		port map (
			alt_vip_itc_0_clocked_video_vid_clk       => CONNECTED_TO_alt_vip_itc_0_clocked_video_vid_clk,       --      alt_vip_itc_0_clocked_video.vid_clk
			alt_vip_itc_0_clocked_video_vid_data      => CONNECTED_TO_alt_vip_itc_0_clocked_video_vid_data,      --                                 .vid_data
			alt_vip_itc_0_clocked_video_underflow     => CONNECTED_TO_alt_vip_itc_0_clocked_video_underflow,     --                                 .underflow
			alt_vip_itc_0_clocked_video_vid_datavalid => CONNECTED_TO_alt_vip_itc_0_clocked_video_vid_datavalid, --                                 .vid_datavalid
			alt_vip_itc_0_clocked_video_vid_v_sync    => CONNECTED_TO_alt_vip_itc_0_clocked_video_vid_v_sync,    --                                 .vid_v_sync
			alt_vip_itc_0_clocked_video_vid_h_sync    => CONNECTED_TO_alt_vip_itc_0_clocked_video_vid_h_sync,    --                                 .vid_h_sync
			alt_vip_itc_0_clocked_video_vid_f         => CONNECTED_TO_alt_vip_itc_0_clocked_video_vid_f,         --                                 .vid_f
			alt_vip_itc_0_clocked_video_vid_h         => CONNECTED_TO_alt_vip_itc_0_clocked_video_vid_h,         --                                 .vid_h
			alt_vip_itc_0_clocked_video_vid_v         => CONNECTED_TO_alt_vip_itc_0_clocked_video_vid_v,         --                                 .vid_v
			clk_clk                                   => CONNECTED_TO_clk_clk,                                   --                              clk.clk
			clk_sdram_clk                             => CONNECTED_TO_clk_sdram_clk,                             --                        clk_sdram.clk
			clk_vga_clk                               => CONNECTED_TO_clk_vga_clk,                               --                          clk_vga.clk
			d8m_xclkin_clk                            => CONNECTED_TO_d8m_xclkin_clk,                            --                       d8m_xclkin.clk
			hps_io_hps_io_emac1_inst_TX_CLK           => CONNECTED_TO_hps_io_hps_io_emac1_inst_TX_CLK,           --                           hps_io.hps_io_emac1_inst_TX_CLK
			hps_io_hps_io_emac1_inst_TXD0             => CONNECTED_TO_hps_io_hps_io_emac1_inst_TXD0,             --                                 .hps_io_emac1_inst_TXD0
			hps_io_hps_io_emac1_inst_TXD1             => CONNECTED_TO_hps_io_hps_io_emac1_inst_TXD1,             --                                 .hps_io_emac1_inst_TXD1
			hps_io_hps_io_emac1_inst_TXD2             => CONNECTED_TO_hps_io_hps_io_emac1_inst_TXD2,             --                                 .hps_io_emac1_inst_TXD2
			hps_io_hps_io_emac1_inst_TXD3             => CONNECTED_TO_hps_io_hps_io_emac1_inst_TXD3,             --                                 .hps_io_emac1_inst_TXD3
			hps_io_hps_io_emac1_inst_RXD0             => CONNECTED_TO_hps_io_hps_io_emac1_inst_RXD0,             --                                 .hps_io_emac1_inst_RXD0
			hps_io_hps_io_emac1_inst_MDIO             => CONNECTED_TO_hps_io_hps_io_emac1_inst_MDIO,             --                                 .hps_io_emac1_inst_MDIO
			hps_io_hps_io_emac1_inst_MDC              => CONNECTED_TO_hps_io_hps_io_emac1_inst_MDC,              --                                 .hps_io_emac1_inst_MDC
			hps_io_hps_io_emac1_inst_RX_CTL           => CONNECTED_TO_hps_io_hps_io_emac1_inst_RX_CTL,           --                                 .hps_io_emac1_inst_RX_CTL
			hps_io_hps_io_emac1_inst_TX_CTL           => CONNECTED_TO_hps_io_hps_io_emac1_inst_TX_CTL,           --                                 .hps_io_emac1_inst_TX_CTL
			hps_io_hps_io_emac1_inst_RX_CLK           => CONNECTED_TO_hps_io_hps_io_emac1_inst_RX_CLK,           --                                 .hps_io_emac1_inst_RX_CLK
			hps_io_hps_io_emac1_inst_RXD1             => CONNECTED_TO_hps_io_hps_io_emac1_inst_RXD1,             --                                 .hps_io_emac1_inst_RXD1
			hps_io_hps_io_emac1_inst_RXD2             => CONNECTED_TO_hps_io_hps_io_emac1_inst_RXD2,             --                                 .hps_io_emac1_inst_RXD2
			hps_io_hps_io_emac1_inst_RXD3             => CONNECTED_TO_hps_io_hps_io_emac1_inst_RXD3,             --                                 .hps_io_emac1_inst_RXD3
			hps_io_hps_io_qspi_inst_IO0               => CONNECTED_TO_hps_io_hps_io_qspi_inst_IO0,               --                                 .hps_io_qspi_inst_IO0
			hps_io_hps_io_qspi_inst_IO1               => CONNECTED_TO_hps_io_hps_io_qspi_inst_IO1,               --                                 .hps_io_qspi_inst_IO1
			hps_io_hps_io_qspi_inst_IO2               => CONNECTED_TO_hps_io_hps_io_qspi_inst_IO2,               --                                 .hps_io_qspi_inst_IO2
			hps_io_hps_io_qspi_inst_IO3               => CONNECTED_TO_hps_io_hps_io_qspi_inst_IO3,               --                                 .hps_io_qspi_inst_IO3
			hps_io_hps_io_qspi_inst_SS0               => CONNECTED_TO_hps_io_hps_io_qspi_inst_SS0,               --                                 .hps_io_qspi_inst_SS0
			hps_io_hps_io_qspi_inst_CLK               => CONNECTED_TO_hps_io_hps_io_qspi_inst_CLK,               --                                 .hps_io_qspi_inst_CLK
			hps_io_hps_io_sdio_inst_CMD               => CONNECTED_TO_hps_io_hps_io_sdio_inst_CMD,               --                                 .hps_io_sdio_inst_CMD
			hps_io_hps_io_sdio_inst_D0                => CONNECTED_TO_hps_io_hps_io_sdio_inst_D0,                --                                 .hps_io_sdio_inst_D0
			hps_io_hps_io_sdio_inst_D1                => CONNECTED_TO_hps_io_hps_io_sdio_inst_D1,                --                                 .hps_io_sdio_inst_D1
			hps_io_hps_io_sdio_inst_CLK               => CONNECTED_TO_hps_io_hps_io_sdio_inst_CLK,               --                                 .hps_io_sdio_inst_CLK
			hps_io_hps_io_sdio_inst_D2                => CONNECTED_TO_hps_io_hps_io_sdio_inst_D2,                --                                 .hps_io_sdio_inst_D2
			hps_io_hps_io_sdio_inst_D3                => CONNECTED_TO_hps_io_hps_io_sdio_inst_D3,                --                                 .hps_io_sdio_inst_D3
			hps_io_hps_io_usb1_inst_D0                => CONNECTED_TO_hps_io_hps_io_usb1_inst_D0,                --                                 .hps_io_usb1_inst_D0
			hps_io_hps_io_usb1_inst_D1                => CONNECTED_TO_hps_io_hps_io_usb1_inst_D1,                --                                 .hps_io_usb1_inst_D1
			hps_io_hps_io_usb1_inst_D2                => CONNECTED_TO_hps_io_hps_io_usb1_inst_D2,                --                                 .hps_io_usb1_inst_D2
			hps_io_hps_io_usb1_inst_D3                => CONNECTED_TO_hps_io_hps_io_usb1_inst_D3,                --                                 .hps_io_usb1_inst_D3
			hps_io_hps_io_usb1_inst_D4                => CONNECTED_TO_hps_io_hps_io_usb1_inst_D4,                --                                 .hps_io_usb1_inst_D4
			hps_io_hps_io_usb1_inst_D5                => CONNECTED_TO_hps_io_hps_io_usb1_inst_D5,                --                                 .hps_io_usb1_inst_D5
			hps_io_hps_io_usb1_inst_D6                => CONNECTED_TO_hps_io_hps_io_usb1_inst_D6,                --                                 .hps_io_usb1_inst_D6
			hps_io_hps_io_usb1_inst_D7                => CONNECTED_TO_hps_io_hps_io_usb1_inst_D7,                --                                 .hps_io_usb1_inst_D7
			hps_io_hps_io_usb1_inst_CLK               => CONNECTED_TO_hps_io_hps_io_usb1_inst_CLK,               --                                 .hps_io_usb1_inst_CLK
			hps_io_hps_io_usb1_inst_STP               => CONNECTED_TO_hps_io_hps_io_usb1_inst_STP,               --                                 .hps_io_usb1_inst_STP
			hps_io_hps_io_usb1_inst_DIR               => CONNECTED_TO_hps_io_hps_io_usb1_inst_DIR,               --                                 .hps_io_usb1_inst_DIR
			hps_io_hps_io_usb1_inst_NXT               => CONNECTED_TO_hps_io_hps_io_usb1_inst_NXT,               --                                 .hps_io_usb1_inst_NXT
			hps_io_hps_io_spim1_inst_CLK              => CONNECTED_TO_hps_io_hps_io_spim1_inst_CLK,              --                                 .hps_io_spim1_inst_CLK
			hps_io_hps_io_spim1_inst_MOSI             => CONNECTED_TO_hps_io_hps_io_spim1_inst_MOSI,             --                                 .hps_io_spim1_inst_MOSI
			hps_io_hps_io_spim1_inst_MISO             => CONNECTED_TO_hps_io_hps_io_spim1_inst_MISO,             --                                 .hps_io_spim1_inst_MISO
			hps_io_hps_io_spim1_inst_SS0              => CONNECTED_TO_hps_io_hps_io_spim1_inst_SS0,              --                                 .hps_io_spim1_inst_SS0
			hps_io_hps_io_uart0_inst_RX               => CONNECTED_TO_hps_io_hps_io_uart0_inst_RX,               --                                 .hps_io_uart0_inst_RX
			hps_io_hps_io_uart0_inst_TX               => CONNECTED_TO_hps_io_hps_io_uart0_inst_TX,               --                                 .hps_io_uart0_inst_TX
			hps_io_hps_io_i2c0_inst_SDA               => CONNECTED_TO_hps_io_hps_io_i2c0_inst_SDA,               --                                 .hps_io_i2c0_inst_SDA
			hps_io_hps_io_i2c0_inst_SCL               => CONNECTED_TO_hps_io_hps_io_i2c0_inst_SCL,               --                                 .hps_io_i2c0_inst_SCL
			hps_io_hps_io_i2c1_inst_SDA               => CONNECTED_TO_hps_io_hps_io_i2c1_inst_SDA,               --                                 .hps_io_i2c1_inst_SDA
			hps_io_hps_io_i2c1_inst_SCL               => CONNECTED_TO_hps_io_hps_io_i2c1_inst_SCL,               --                                 .hps_io_i2c1_inst_SCL
			hps_io_hps_io_gpio_inst_GPIO09            => CONNECTED_TO_hps_io_hps_io_gpio_inst_GPIO09,            --                                 .hps_io_gpio_inst_GPIO09
			hps_io_hps_io_gpio_inst_GPIO35            => CONNECTED_TO_hps_io_hps_io_gpio_inst_GPIO35,            --                                 .hps_io_gpio_inst_GPIO35
			hps_io_hps_io_gpio_inst_GPIO40            => CONNECTED_TO_hps_io_hps_io_gpio_inst_GPIO40,            --                                 .hps_io_gpio_inst_GPIO40
			hps_io_hps_io_gpio_inst_GPIO41            => CONNECTED_TO_hps_io_hps_io_gpio_inst_GPIO41,            --                                 .hps_io_gpio_inst_GPIO41
			hps_io_hps_io_gpio_inst_GPIO48            => CONNECTED_TO_hps_io_hps_io_gpio_inst_GPIO48,            --                                 .hps_io_gpio_inst_GPIO48
			hps_io_hps_io_gpio_inst_GPIO53            => CONNECTED_TO_hps_io_hps_io_gpio_inst_GPIO53,            --                                 .hps_io_gpio_inst_GPIO53
			hps_io_hps_io_gpio_inst_GPIO54            => CONNECTED_TO_hps_io_hps_io_gpio_inst_GPIO54,            --                                 .hps_io_gpio_inst_GPIO54
			hps_io_hps_io_gpio_inst_GPIO61            => CONNECTED_TO_hps_io_hps_io_gpio_inst_GPIO61,            --                                 .hps_io_gpio_inst_GPIO61
			i2c_opencores_camera_export_scl_pad_io    => CONNECTED_TO_i2c_opencores_camera_export_scl_pad_io,    --      i2c_opencores_camera_export.scl_pad_io
			i2c_opencores_camera_export_sda_pad_io    => CONNECTED_TO_i2c_opencores_camera_export_sda_pad_io,    --                                 .sda_pad_io
			i2c_opencores_mipi_export_scl_pad_io      => CONNECTED_TO_i2c_opencores_mipi_export_scl_pad_io,      --        i2c_opencores_mipi_export.scl_pad_io
			i2c_opencores_mipi_export_sda_pad_io      => CONNECTED_TO_i2c_opencores_mipi_export_sda_pad_io,      --                                 .sda_pad_io
			key_external_connection_export            => CONNECTED_TO_key_external_connection_export,            --          key_external_connection.export
			led_external_connection_export            => CONNECTED_TO_led_external_connection_export,            --          led_external_connection.export
			memory_mem_a                              => CONNECTED_TO_memory_mem_a,                              --                           memory.mem_a
			memory_mem_ba                             => CONNECTED_TO_memory_mem_ba,                             --                                 .mem_ba
			memory_mem_ck                             => CONNECTED_TO_memory_mem_ck,                             --                                 .mem_ck
			memory_mem_ck_n                           => CONNECTED_TO_memory_mem_ck_n,                           --                                 .mem_ck_n
			memory_mem_cke                            => CONNECTED_TO_memory_mem_cke,                            --                                 .mem_cke
			memory_mem_cs_n                           => CONNECTED_TO_memory_mem_cs_n,                           --                                 .mem_cs_n
			memory_mem_ras_n                          => CONNECTED_TO_memory_mem_ras_n,                          --                                 .mem_ras_n
			memory_mem_cas_n                          => CONNECTED_TO_memory_mem_cas_n,                          --                                 .mem_cas_n
			memory_mem_we_n                           => CONNECTED_TO_memory_mem_we_n,                           --                                 .mem_we_n
			memory_mem_reset_n                        => CONNECTED_TO_memory_mem_reset_n,                        --                                 .mem_reset_n
			memory_mem_dq                             => CONNECTED_TO_memory_mem_dq,                             --                                 .mem_dq
			memory_mem_dqs                            => CONNECTED_TO_memory_mem_dqs,                            --                                 .mem_dqs
			memory_mem_dqs_n                          => CONNECTED_TO_memory_mem_dqs_n,                          --                                 .mem_dqs_n
			memory_mem_odt                            => CONNECTED_TO_memory_mem_odt,                            --                                 .mem_odt
			memory_mem_dm                             => CONNECTED_TO_memory_mem_dm,                             --                                 .mem_dm
			memory_oct_rzqin                          => CONNECTED_TO_memory_oct_rzqin,                          --                                 .oct_rzqin
			mipi_pwdn_n_external_connection_export    => CONNECTED_TO_mipi_pwdn_n_external_connection_export,    --  mipi_pwdn_n_external_connection.export
			mipi_reset_n_external_connection_export   => CONNECTED_TO_mipi_reset_n_external_connection_export,   -- mipi_reset_n_external_connection.export
			reset_reset_n                             => CONNECTED_TO_reset_reset_n,                             --                            reset.reset_n
			sdram_wire_addr                           => CONNECTED_TO_sdram_wire_addr,                           --                       sdram_wire.addr
			sdram_wire_ba                             => CONNECTED_TO_sdram_wire_ba,                             --                                 .ba
			sdram_wire_cas_n                          => CONNECTED_TO_sdram_wire_cas_n,                          --                                 .cas_n
			sdram_wire_cke                            => CONNECTED_TO_sdram_wire_cke,                            --                                 .cke
			sdram_wire_cs_n                           => CONNECTED_TO_sdram_wire_cs_n,                           --                                 .cs_n
			sdram_wire_dq                             => CONNECTED_TO_sdram_wire_dq,                             --                                 .dq
			sdram_wire_dqm                            => CONNECTED_TO_sdram_wire_dqm,                            --                                 .dqm
			sdram_wire_ras_n                          => CONNECTED_TO_sdram_wire_ras_n,                          --                                 .ras_n
			sdram_wire_we_n                           => CONNECTED_TO_sdram_wire_we_n,                           --                                 .we_n
			sw_external_connection_export             => CONNECTED_TO_sw_external_connection_export,             --           sw_external_connection.export
			terasic_auto_focus_0_conduit_vcm_i2c_sda  => CONNECTED_TO_terasic_auto_focus_0_conduit_vcm_i2c_sda,  --     terasic_auto_focus_0_conduit.vcm_i2c_sda
			terasic_auto_focus_0_conduit_clk50        => CONNECTED_TO_terasic_auto_focus_0_conduit_clk50,        --                                 .clk50
			terasic_auto_focus_0_conduit_vcm_i2c_scl  => CONNECTED_TO_terasic_auto_focus_0_conduit_vcm_i2c_scl,  --                                 .vcm_i2c_scl
			terasic_camera_0_conduit_end_D            => CONNECTED_TO_terasic_camera_0_conduit_end_D,            --     terasic_camera_0_conduit_end.D
			terasic_camera_0_conduit_end_FVAL         => CONNECTED_TO_terasic_camera_0_conduit_end_FVAL,         --                                 .FVAL
			terasic_camera_0_conduit_end_LVAL         => CONNECTED_TO_terasic_camera_0_conduit_end_LVAL,         --                                 .LVAL
			terasic_camera_0_conduit_end_PIXCLK       => CONNECTED_TO_terasic_camera_0_conduit_end_PIXCLK,       --                                 .PIXCLK
			touch_uart_RXD                            => CONNECTED_TO_touch_uart_RXD,                            --                       touch_uart.RXD
			touch_uart_TXD                            => CONNECTED_TO_touch_uart_TXD,                            --                                 .TXD
			hexes_export                              => CONNECTED_TO_hexes_export                               --                            hexes.export
		);
