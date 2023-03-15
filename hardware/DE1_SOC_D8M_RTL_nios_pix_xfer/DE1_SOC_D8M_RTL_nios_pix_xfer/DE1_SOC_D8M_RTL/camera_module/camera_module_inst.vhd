	component camera_module is
		port (
			clk_clk                                    : in  std_logic                     := 'X';             -- clk
			img_cpu_reader_0_cpu_rdy_cpu_rdy           : out std_logic;                                        -- cpu_rdy
			img_cpu_reader_0_get_next_pix_get_next_pix : out std_logic;                                        -- get_next_pix
			img_cpu_reader_0_img_done_img_done         : in  std_logic                     := 'X';             -- img_done
			img_cpu_reader_0_out_state_out_state       : out std_logic_vector(3 downto 0);                     -- out_state
			img_cpu_reader_0_pix_data_pixel_data       : in  std_logic_vector(23 downto 0) := (others => 'X'); -- pixel_data
			img_cpu_reader_0_pix_rdy_pix_rdy           : in  std_logic                     := 'X';             -- pix_rdy
			img_cpu_reader_0_pix_rdy_out_pix_rdy_out   : out std_logic_vector(31 downto 0);                    -- pix_rdy_out
			img_cpu_reader_0_pix_rgb_out_pix_rgb_out   : out std_logic_vector(23 downto 0);                    -- pix_rgb_out
			pio_in_export                              : in  std_logic                     := 'X';             -- export
			reset_reset_n                              : in  std_logic                     := 'X'              -- reset_n
		);
	end component camera_module;

	u0 : component camera_module
		port map (
			clk_clk                                    => CONNECTED_TO_clk_clk,                                    --                           clk.clk
			img_cpu_reader_0_cpu_rdy_cpu_rdy           => CONNECTED_TO_img_cpu_reader_0_cpu_rdy_cpu_rdy,           --      img_cpu_reader_0_cpu_rdy.cpu_rdy
			img_cpu_reader_0_get_next_pix_get_next_pix => CONNECTED_TO_img_cpu_reader_0_get_next_pix_get_next_pix, -- img_cpu_reader_0_get_next_pix.get_next_pix
			img_cpu_reader_0_img_done_img_done         => CONNECTED_TO_img_cpu_reader_0_img_done_img_done,         --     img_cpu_reader_0_img_done.img_done
			img_cpu_reader_0_out_state_out_state       => CONNECTED_TO_img_cpu_reader_0_out_state_out_state,       --    img_cpu_reader_0_out_state.out_state
			img_cpu_reader_0_pix_data_pixel_data       => CONNECTED_TO_img_cpu_reader_0_pix_data_pixel_data,       --     img_cpu_reader_0_pix_data.pixel_data
			img_cpu_reader_0_pix_rdy_pix_rdy           => CONNECTED_TO_img_cpu_reader_0_pix_rdy_pix_rdy,           --      img_cpu_reader_0_pix_rdy.pix_rdy
			img_cpu_reader_0_pix_rdy_out_pix_rdy_out   => CONNECTED_TO_img_cpu_reader_0_pix_rdy_out_pix_rdy_out,   --  img_cpu_reader_0_pix_rdy_out.pix_rdy_out
			img_cpu_reader_0_pix_rgb_out_pix_rgb_out   => CONNECTED_TO_img_cpu_reader_0_pix_rgb_out_pix_rgb_out,   --  img_cpu_reader_0_pix_rgb_out.pix_rgb_out
			pio_in_export                              => CONNECTED_TO_pio_in_export,                              --                        pio_in.export
			reset_reset_n                              => CONNECTED_TO_reset_reset_n                               --                         reset.reset_n
		);

