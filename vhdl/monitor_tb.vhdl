-- *** MOCS-COPYRIGHT-NOTICE-BEGIN ***
--
-- This copyright notice is auto-generated by ./add-copyright-notice.
-- Additional copyright notices must be added below the last line of this notice.
--
-- MoCS (https://lewis.cs.uni-saarland.de/tools/mocs/): "vhdl/monitor_tb.vhdl".
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
use work.general.all;
use work.monitor;
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.all;

 --  A testbench has no ports.
entity monitor_tb is
end entity monitor_tb;
 
 architecture behav of monitor_tb is
    signal CLOCK, output : STD_LOGIC; 
    signal p : unsigned(prop_range);
 begin
    --  Component instantiation.
     monitor_0: entity work.monitor(Behavioral) 
    port map (CLOCK, p, output);

    clk : process
    begin
        CLOCK <= '0'; wait for 5 ns;
        CLOCK <= '1'; wait for 5 ns;
    end process;
 
    stimulus0 : process is
    begin
        p(0) <= '0';
        wait for 11 ns; p(0) <= '1';
        wait for 9 ns; p(0) <= '0';

        wait for 33 ns; p(0) <= '1';
        wait for 9 ns; p(0) <= '0';

        wait for 35 ns; p(0) <= '1';
        wait for 20 ns; p(0) <= '0';
        wait;
    end process stimulus0;
	 
    stimulus1 : process is
    begin
        p(1) <= '0'; 
        wait for 27 ns; p(1) <= '1';
        wait for 7 ns; p(1) <= '0';

        --wait for 8 ns; p(1) <= '1';
        --wait for 7 ns; p(1) <= '0';

        wait for 93 ns; p(1) <= '1';
        wait for 20 ns; p(1) <= '0';
        wait;
    end process stimulus1;
 end behav;

