-- *** MOCS-COPYRIGHT-NOTICE-BEGIN ***
--
-- This copyright notice is auto-generated by ./add-copyright-notice.
-- Additional copyright notices must be added below the last line of this notice.
--
-- MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/): "vhdl/pipeline_tb.vhdl".
-- The content of this file is copyright of Saarland University -
-- Copyright (C) 2009 Saarland University, Reactive Systems Group, Lars Kuhtz <kuhtz@cs.uni-sb.de>.
--
-- This file is part of MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/).
--
-- License: three-clause BSD style license.
-- The license text can be found in the file LICENSE.
--
-- *** MOCS-COPYRIGHT-NOTICE-END ***

-- ----------------------------------------------------------------- --
-- use work.general.all;
library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.numeric_std.all;
use work.pipeline;

-- the pipeline for a single prop
entity pipeline_tb is
end pipeline_tb;

architecture Behavioral of pipeline_tb is
    signal CLOCK : STD_LOGIC;
    signal i     : STD_LOGIC;
    signal o     : unsigned(5 downto 0);

begin

    pipeline0: entity work.pipeline(Behavioral)
    generic map (LENGTH => 5)
    port map (CLOCK,i,o);
    
    clk: process
    begin 
        CLOCK <= '1'; wait for 5 ns;
        CLOCK <= '0'; wait for 5 ns;
    end process;
    
    stimulus: process
    begin
        i <= '0'; wait for 8 ns;
        i <= '1'; wait for 11 ns;
        i <= '0'; wait for 13 ns;
        i <= '1'; wait for 3 ns;
        wait;
    end process;
end Behavioral;
