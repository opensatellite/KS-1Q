# Written by Synplify Pro version mapact, Build 1659R. Synopsys Run ID: sid1477896334 
# Top Level Design Parameters 

# Clocks 
create_clock -period 10.000 -waveform {0.000 5.000} -name {divider|Bit_Count_16dec_inferred_clock[3]} [get_pins {divider_0/Bit_Count_16dec[3]:Q}] 
create_clock -period 10.000 -waveform {0.000 5.000} -name {test_4463andcpuopration_MSS|SPI_0_CLK_M2F_inferred_clock} [get_pins {test_4463andcpuopration_MSS_0/MSS_ADLIB_INST:SPI0_CLK_OUT}] 
create_clock -period 10.000 -waveform {0.000 5.000} -name {test_4463andcpuopration|RX_GPIO1} [get_ports {RX_GPIO1}] 
create_clock -period 10.000 -waveform {0.000 5.000} -name {divider|Bit_Count_2dec_inferred_clock} [get_pins {divider_0/Bit_Count_2dec:Q}] 
create_clock -period 10.000 -waveform {0.000 5.000} -name {test_4463andcpuopration|TX_GPIO1} [get_ports {TX_GPIO1}] 

# Virtual Clocks 

# Generated Clocks 

# Paths Between Clocks 

# Multicycle Constraints 

# Point-to-point Delay Constraints 

# False Path Constraints 

# Output Load Constraints 

# Driving Cell Constraints 

# Input Delay Constraints 

# Output Delay Constraints 

# Wire Loads 

# Other Constraints 

# syn_hier Attributes 

# set_case Attributes 

# Clock Delay Constraints 
set Inferred_clkgroup_3 [list test_4463andcpuopration|RX_GPIO1]
set Inferred_clkgroup_5 [list test_4463andcpuopration|TX_GPIO1]
set Inferred_clkgroup_2 [list test_4463andcpuopration_MSS|SPI_0_CLK_M2F_inferred_clock]
set Inferred_clkgroup_4 [list divider|Bit_Count_2dec_inferred_clock]
set Inferred_clkgroup_0 [list divider|Bit_Count_16dec_inferred_clock\[3\]]
set_clock_groups -asynchronous -group $Inferred_clkgroup_3
set_clock_groups -asynchronous -group $Inferred_clkgroup_5
set_clock_groups -asynchronous -group $Inferred_clkgroup_2
set_clock_groups -asynchronous -group $Inferred_clkgroup_4
set_clock_groups -asynchronous -group $Inferred_clkgroup_0


# syn_mode Attributes 

# Cells 

# Port DRC Rules 

# Input Transition Constraints 

# Unused constraints (intentionally commented out) 

# Non-forward-annotatable constraints (intentionally commented out) 

# Block Path constraints 

