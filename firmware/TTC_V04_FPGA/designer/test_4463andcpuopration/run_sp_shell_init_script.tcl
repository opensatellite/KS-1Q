set_device \
    -family  SmartFusion2 \
    -die     PA4M1000_N \
    -package tq144 \
    -speed   STD \
    -tempr   {COM} \
    -voltr   {COM}
set_def {VOLTAGE} {1.2}
set_def {VCCI_1.2_VOLTR} {COM}
set_def {VCCI_1.5_VOLTR} {COM}
set_def {VCCI_1.8_VOLTR} {COM}
set_def {VCCI_2.5_VOLTR} {COM}
set_def {VCCI_3.3_VOLTR} {COM}
set_def {PLL_SUPPLY} {PLL_SUPPLY_33}
set_def {PA4_URAM_FF_CONFIG} {SUSPEND}
set_def {PA4_SRAM_FF_CONFIG} {SUSPEND}
set_def {PA4_MSS_FF_CLOCK} {RCOSC_1MHZ}
set_netlist -afl {C:\Users\Administrator\Desktop\workplace\designer\test_4463andcpuopration\test_4463andcpuopration.afl} -adl {C:\Users\Administrator\Desktop\workplace\designer\test_4463andcpuopration\test_4463andcpuopration.adl}
set_placement   {C:\Users\Administrator\Desktop\workplace\designer\test_4463andcpuopration\test_4463andcpuopration.loc}
set_routing     {C:\Users\Administrator\Desktop\workplace\designer\test_4463andcpuopration\test_4463andcpuopration.seg}
