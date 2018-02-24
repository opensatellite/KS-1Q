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
set_def {RTG4_MITIGATION_ON} {0}
set_def NETLIST_TYPE EDIF
set_name test_4463andcpuopration
set_workdir {C:\Users\Administrator\Desktop\workplace\designer\test_4463andcpuopration}
set_log     {C:\Users\Administrator\Desktop\workplace\designer\test_4463andcpuopration\test_4463andcpuopration_sdc.log}
set_design_state pre_layout
