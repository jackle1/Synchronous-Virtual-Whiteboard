	component cpu is
		port (
			leds_r_export               : out   std_logic_vector(7 downto 0);                     -- export
			sdram_addr                  : out   std_logic_vector(12 downto 0);                    -- addr
			sdram_ba                    : out   std_logic_vector(1 downto 0);                     -- ba
			sdram_cas_n                 : out   std_logic;                                        -- cas_n
			sdram_cke                   : out   std_logic;                                        -- cke
			sdram_cs_n                  : out   std_logic;                                        -- cs_n
			sdram_dq                    : inout std_logic_vector(15 downto 0) := (others => 'X'); -- dq
			sdram_dqm                   : out   std_logic_vector(1 downto 0);                     -- dqm
			sdram_ras_n                 : out   std_logic;                                        -- ras_n
			sdram_we_n                  : out   std_logic;                                        -- we_n
			sdram_clk_clk               : out   std_logic;                                        -- clk
			sys_pll_clk_ref_clk_clk     : in    std_logic                     := 'X';             -- clk
			sys_pll_clk_ref_reset_reset : in    std_logic                     := 'X';             -- reset
			touch_uart_RXD              : in    std_logic                     := 'X';             -- RXD
			touch_uart_TXD              : out   std_logic;                                        -- TXD
			vga_CLK                     : out   std_logic;                                        -- CLK
			vga_HS                      : out   std_logic;                                        -- HS
			vga_VS                      : out   std_logic;                                        -- VS
			vga_BLANK                   : out   std_logic;                                        -- BLANK
			vga_SYNC                    : out   std_logic;                                        -- SYNC
			vga_R                       : out   std_logic_vector(7 downto 0);                     -- R
			vga_G                       : out   std_logic_vector(7 downto 0);                     -- G
			vga_B                       : out   std_logic_vector(7 downto 0);                     -- B
			wifi_uart_RXD               : in    std_logic                     := 'X';             -- RXD
			wifi_uart_TXD               : out   std_logic                                         -- TXD
		);
	end component cpu;

	u0 : component cpu
		port map (
			leds_r_export               => CONNECTED_TO_leds_r_export,               --                leds_r.export
			sdram_addr                  => CONNECTED_TO_sdram_addr,                  --                 sdram.addr
			sdram_ba                    => CONNECTED_TO_sdram_ba,                    --                      .ba
			sdram_cas_n                 => CONNECTED_TO_sdram_cas_n,                 --                      .cas_n
			sdram_cke                   => CONNECTED_TO_sdram_cke,                   --                      .cke
			sdram_cs_n                  => CONNECTED_TO_sdram_cs_n,                  --                      .cs_n
			sdram_dq                    => CONNECTED_TO_sdram_dq,                    --                      .dq
			sdram_dqm                   => CONNECTED_TO_sdram_dqm,                   --                      .dqm
			sdram_ras_n                 => CONNECTED_TO_sdram_ras_n,                 --                      .ras_n
			sdram_we_n                  => CONNECTED_TO_sdram_we_n,                  --                      .we_n
			sdram_clk_clk               => CONNECTED_TO_sdram_clk_clk,               --             sdram_clk.clk
			sys_pll_clk_ref_clk_clk     => CONNECTED_TO_sys_pll_clk_ref_clk_clk,     --   sys_pll_clk_ref_clk.clk
			sys_pll_clk_ref_reset_reset => CONNECTED_TO_sys_pll_clk_ref_reset_reset, -- sys_pll_clk_ref_reset.reset
			touch_uart_RXD              => CONNECTED_TO_touch_uart_RXD,              --            touch_uart.RXD
			touch_uart_TXD              => CONNECTED_TO_touch_uart_TXD,              --                      .TXD
			vga_CLK                     => CONNECTED_TO_vga_CLK,                     --                   vga.CLK
			vga_HS                      => CONNECTED_TO_vga_HS,                      --                      .HS
			vga_VS                      => CONNECTED_TO_vga_VS,                      --                      .VS
			vga_BLANK                   => CONNECTED_TO_vga_BLANK,                   --                      .BLANK
			vga_SYNC                    => CONNECTED_TO_vga_SYNC,                    --                      .SYNC
			vga_R                       => CONNECTED_TO_vga_R,                       --                      .R
			vga_G                       => CONNECTED_TO_vga_G,                       --                      .G
			vga_B                       => CONNECTED_TO_vga_B,                       --                      .B
			wifi_uart_RXD               => CONNECTED_TO_wifi_uart_RXD,               --             wifi_uart.RXD
			wifi_uart_TXD               => CONNECTED_TO_wifi_uart_TXD                --                      .TXD
		);

