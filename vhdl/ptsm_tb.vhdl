-- *** MOCS-COPYRIGHT-NOTICE-BEGIN ***
--
-- This copyright notice is auto-generated by ./add-copyright-notice.
-- Additional copyright notices must be added below the last line of this notice.
--
-- MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/): "vhdl/ptsm_tb.vhdl".
-- The content of this file is copyright of Saarland University -
-- Copyright (C) 2009 Saarland University, Reactive Systems Group, Lars Kuhtz.
--
-- This file is part of MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/).
--
-- License: three-clause BSD style license.
-- The license text can be found in the file LICENSE.
--
-- *** MOCS-COPYRIGHT-NOTICE-END ***

-- ----------------------------------------------------------------- --
use work.general.all;
use work.ptsm;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.all;

 --  A testbench has no ports.
entity ptsm_tb is
end entity ptsm_tb;
 
 architecture Behavioral of ptsm_tb is

    signal CLOCK, output : STD_LOGIC; 
    signal i : unsigned(subf_range);
    signal f : STD_LOGIC; -- false_state
    signal o : STD_LOGIC; -- output (current state)

 begin

    ptsm0: entity work.ptsm(Behavioral)
    port map(Clock, i, f, o);

    clk : process
    begin
        CLOCK <= '0'; wait for 5 ns;
        CLOCK <= '1'; wait for 5 ns;
    end process;
 
    --  This process does the real job.
    stimulus : process is
    begin
        -- TODO Loop over subf_range with random values as generic behavior
        i(0) <= '0'; wait for 1 ns;
        i(0) <= '1'; wait for 23 ns;
        i(0) <= '1'; wait for 4 ns;
        i(0) <= '0'; wait for 20 ns;
        i(0) <= '1'; wait for 10 ns;
        i(0) <= '0'; wait for 23 ns;
        i(0) <= '1'; wait for 20 ns;
       wait;
    end process stimulus;
 end Behavioral;

