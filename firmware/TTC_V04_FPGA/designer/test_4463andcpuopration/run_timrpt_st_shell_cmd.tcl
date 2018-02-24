set max_slow_lv_ht      "not_run"
set min_fast_hv_lt      "not_run"
set max_fast_hv_lt      "not_run"
set min_slow_lv_ht      "not_run"
set max_viol_slow_lv_ht "not_run"
set min_viol_fast_hv_lt "not_run"
set max_viol_fast_hv_lt "not_run"
set min_viol_slow_lv_ht "not_run"
set coverage            "not_run"
set_options -max_opcond worst
set_options -min_opcond best
set max_slow_lv_ht [report \
    -type     timing \
    -analysis max \
    -format   xml \
    {C:\Users\Administrator\Desktop\10.14_v04workplace\v04workplace\designer\test_4463andcpuopration\test_4463andcpuopration_max_timing_slow_1.14V_100C.xml} ]
set min_fast_hv_lt [report \
    -type     timing \
    -analysis min \
    -format   xml \
    {C:\Users\Administrator\Desktop\10.14_v04workplace\v04workplace\designer\test_4463andcpuopration\test_4463andcpuopration_min_timing_fast_1.26V_-40C.xml} ]
set max_viol_slow_lv_ht [report \
    -type     timing_violations \
    -analysis max \
    -format   xml \
    {C:\Users\Administrator\Desktop\10.14_v04workplace\v04workplace\designer\test_4463andcpuopration\test_4463andcpuopration_max_timing_violations_slow_1.14V_100C.xml} ]
set min_viol_fast_hv_lt [report \
    -type     timing_violations \
    -analysis min \
    -format   xml \
    {C:\Users\Administrator\Desktop\10.14_v04workplace\v04workplace\designer\test_4463andcpuopration\test_4463andcpuopration_min_timing_violations_fast_1.26V_-40C.xml} ]
set coverage [report \
    -type     constraints_coverage \
    -format   xml \
    {C:\Users\Administrator\Desktop\10.14_v04workplace\v04workplace\designer\test_4463andcpuopration\test_4463andcpuopration_timing_constraints_coverage.xml} ]
report \
    -type     combinational_loops \
    -format   xml \
    {C:\Users\Administrator\Desktop\10.14_v04workplace\v04workplace\designer\test_4463andcpuopration\test_4463andcpuopration_timing_combinational_loops.xml}
set_options -max_opcond best
set_options -min_opcond worst
set has_violations {C:\Users\Administrator\Desktop\10.14_v04workplace\v04workplace\designer\test_4463andcpuopration\test_4463andcpuopration_has_violations}
set fp [open $has_violations w]
puts $fp "_max_timing_slow_1.14V_100C $max_slow_lv_ht"
puts $fp "_min_timing_fast_1.26V_-40C $min_fast_hv_lt"
puts $fp "_max_timing_fast_1.26V_-40C $max_fast_hv_lt"
puts $fp "_min_timing_slow_1.14V_100C $min_slow_lv_ht"
puts $fp "_max_timing_violations_slow_1.14V_100C $max_viol_slow_lv_ht"
puts $fp "_min_timing_violations_fast_1.26V_-40C $min_viol_fast_hv_lt"
puts $fp "_max_timing_violations_fast_1.26V_-40C $max_viol_fast_hv_lt"
puts $fp "_min_timing_violations_slow_1.14V_100C $min_viol_slow_lv_ht"
puts $fp "_timing_constraints_coverage $coverage"
close $fp
