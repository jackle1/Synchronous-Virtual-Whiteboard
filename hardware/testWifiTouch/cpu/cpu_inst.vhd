	component cpu is
		port (
			clk_clk        : in  std_logic                    := 'X'; -- clk
			reset_reset_n  : in  std_logic                    := 'X'; -- reset_n
			wifi_uart_RXD  : in  std_logic                    := 'X'; -- RXD
			wifi_uart_TXD  : out std_logic;                           -- TXD
			touch_uart_RXD : in  std_logic                    := 'X'; -- RXD
			touch_uart_TXD : out std_logic;                           -- TXD
			leds_r_export  : out std_logic_vector(7 downto 0)         -- export
		);
	end component cpu;

	u0 : component cpu
		port map (
			clk_clk        => CONNECTED_TO_clk_clk,        --        clk.clk
			reset_reset_n  => CONNECTED_TO_reset_reset_n,  --      reset.reset_n
			wifi_uart_RXD  => CONNECTED_TO_wifi_uart_RXD,  --  wifi_uart.RXD
			wifi_uart_TXD  => CONNECTED_TO_wifi_uart_TXD,  --           .TXD
			touch_uart_RXD => CONNECTED_TO_touch_uart_RXD, -- touch_uart.RXD
			touch_uart_TXD => CONNECTED_TO_touch_uart_TXD, --           .TXD
			leds_r_export  => CONNECTED_TO_leds_r_export   --     leds_r.export
		);

