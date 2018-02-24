set inst ""
set cells [ list_objects [ get_cells "*" ] ]
foreach cell $cells \
{
  if { [regexp -nocase {(.*CoreResetP_0)/.*} $cell -> inst] } \
  {
    break
  }
}

if { [string length $inst] != 0 } \
{
  set false_paths \
  {
      {/ddr_settled:CLK                   /ddr_settled_q1:D}
      {/release_sdif0_core:CLK            /release_sdif0_core_q1:D}
      {/release_sdif1_core:CLK            /release_sdif1_core_q1:D}
      {/release_sdif2_core:CLK            /release_sdif2_core_q1:D}
      {/release_sdif3_core:CLK            /release_sdif3_core_q1:D}
      {/MSS_HPMS_READY_int:CLK            /sm0_areset_n_rcosc_q1:ALn}
      {/MSS_HPMS_READY_int:CLK            /sm0_areset_n_rcosc:ALn}
      {/MSS_HPMS_READY_int:CLK            /sdif0_areset_n_rcosc_q1:ALn}
      {/MSS_HPMS_READY_int:CLK            /sdif0_areset_n_rcosc:ALn}
      {/MSS_HPMS_READY_int:CLK            /sdif1_areset_n_rcosc_q1:ALn}
      {/MSS_HPMS_READY_int:CLK            /sdif1_areset_n_rcosc:ALn}
      {/MSS_HPMS_READY_int:CLK            /sdif2_areset_n_rcosc_q1:ALn}
      {/MSS_HPMS_READY_int:CLK            /sdif2_areset_n_rcosc:ALn}
      {/MSS_HPMS_READY_int:CLK            /sdif3_areset_n_rcosc_q1:ALn}
      {/MSS_HPMS_READY_int:CLK            /sdif3_areset_n_rcosc:ALn}
      {/SDIF0_PERST_N_re:CLK              /sdif0_areset_n_rcosc_q1:ALn}
      {/SDIF0_PERST_N_re:CLK              /sdif0_areset_n_rcosc:ALn}
      {/SDIF1_PERST_N_re:CLK              /sdif1_areset_n_rcosc_q1:ALn}
      {/SDIF1_PERST_N_re:CLK              /sdif1_areset_n_rcosc:ALn}
      {/SDIF2_PERST_N_re:CLK              /sdif2_areset_n_rcosc_q1:ALn}
      {/SDIF2_PERST_N_re:CLK              /sdif2_areset_n_rcosc:ALn}
      {/SDIF3_PERST_N_re:CLK              /sdif3_areset_n_rcosc_q1:ALn}
      {/SDIF3_PERST_N_re:CLK              /sdif3_areset_n_rcosc:ALn}
      {/count_sdif0_enable:CLK            /count_sdif0_enable_q1:D}
      {/count_sdif1_enable:CLK            /count_sdif1_enable_q1:D}
      {/count_sdif2_enable:CLK            /count_sdif2_enable_q1:D}
      {/count_sdif3_enable:CLK            /count_sdif3_enable_q1:D}
      {/count_ddr_enable:CLK              /count_ddr_enable_q1:D}
      {/SDIF0_CORE_RESET_N_0:CLK          /genblk2.sdif0_phr/reset_n_q1:ALn}
      {/SDIF0_CORE_RESET_N_0:CLK          /genblk2.sdif0_phr/reset_n_clk_ltssm:ALn}
      {/SDIF1_CORE_RESET_N_0:CLK          /genblk3.sdif1_phr/reset_n_q1:ALn}
      {/SDIF1_CORE_RESET_N_0:CLK          /genblk3.sdif1_phr/reset_n_clk_ltssm:ALn}
      {/SDIF2_CORE_RESET_N_0:CLK          /genblk4.sdif2_phr/reset_n_q1:ALn}
      {/SDIF2_CORE_RESET_N_0:CLK          /genblk4.sdif2_phr/reset_n_clk_ltssm:ALn}
      {/SDIF3_CORE_RESET_N_0:CLK          /genblk5.sdif3_phr/reset_n_q1:ALn}
      {/SDIF3_CORE_RESET_N_0:CLK          /genblk5.sdif3_phr/reset_n_clk_ltssm:ALn}
      {/genblk2.sdif0_phr/hot_reset_n:CLK /genblk2.sdif0_phr/sdif_core_reset_n_q1:ALn}
      {/genblk2.sdif0_phr/hot_reset_n:CLK /genblk2.sdif0_phr/sdif_core_reset_n:ALn}
      {/genblk3.sdif1_phr/hot_reset_n:CLK /genblk3.sdif1_phr/sdif_core_reset_n_q1:ALn}
      {/genblk3.sdif1_phr/hot_reset_n:CLK /genblk3.sdif1_phr/sdif_core_reset_n:ALn}
      {/genblk4.sdif2_phr/hot_reset_n:CLK /genblk4.sdif2_phr/sdif_core_reset_n_q1:ALn}
      {/genblk4.sdif2_phr/hot_reset_n:CLK /genblk4.sdif2_phr/sdif_core_reset_n:ALn}
      {/genblk5.sdif3_phr/hot_reset_n:CLK /genblk5.sdif3_phr/sdif_core_reset_n_q1:ALn}
      {/genblk5.sdif3_phr/hot_reset_n:CLK /genblk5.sdif3_phr/sdif_core_reset_n:ALn}
      {*                                  /genblk2.sdif0_phr/ltssm_q1[0]:D}
      {*                                  /genblk2.sdif0_phr/ltssm_q1[1]:D}
      {*                                  /genblk2.sdif0_phr/ltssm_q1[2]:D}
      {*                                  /genblk2.sdif0_phr/ltssm_q1[3]:D}
      {*                                  /genblk2.sdif0_phr/ltssm_q1[4]:D}
      {*                                  /genblk2.sdif0_phr/psel_q1:D}
      {*                                  /genblk2.sdif0_phr/pwrite_q1:D}
      {*                                  /genblk3.sdif1_phr/ltssm_q1[0]:D}
      {*                                  /genblk3.sdif1_phr/ltssm_q1[1]:D}
      {*                                  /genblk3.sdif1_phr/ltssm_q1[2]:D}
      {*                                  /genblk3.sdif1_phr/ltssm_q1[3]:D}
      {*                                  /genblk3.sdif1_phr/ltssm_q1[4]:D}
      {*                                  /genblk3.sdif1_phr/psel_q1:D}
      {*                                  /genblk3.sdif1_phr/pwrite_q1:D}
      {*                                  /genblk4.sdif2_phr/ltssm_q1[0]:D}
      {*                                  /genblk4.sdif2_phr/ltssm_q1[1]:D}
      {*                                  /genblk4.sdif2_phr/ltssm_q1[2]:D}
      {*                                  /genblk4.sdif2_phr/ltssm_q1[3]:D}
      {*                                  /genblk4.sdif2_phr/ltssm_q1[4]:D}
      {*                                  /genblk4.sdif2_phr/psel_q1:D}
      {*                                  /genblk4.sdif2_phr/pwrite_q1:D}
      {*                                  /genblk5.sdif3_phr/ltssm_q1[0]:D}
      {*                                  /genblk5.sdif3_phr/ltssm_q1[1]:D}
      {*                                  /genblk5.sdif3_phr/ltssm_q1[2]:D}
      {*                                  /genblk5.sdif3_phr/ltssm_q1[3]:D}
      {*                                  /genblk5.sdif3_phr/ltssm_q1[4]:D}
      {*                                  /genblk5.sdif3_phr/psel_q1:D}
      {*                                  /genblk5.sdif3_phr/pwrite_q1:D}
  }

  foreach false_path $false_paths \
  {
    set from_pin [lindex $false_path 0]
    set to_pin   [lindex $false_path 1]

    if { ${from_pin} == "*" } then \
    {
      set pins [ list_objects [ get_pins "$inst${to_pin}" ] ]
      if { [ llength $pins ] == 1 } then \
      {
        set_false_path -to [ get_pins ${pins} ]
      }
    } \
    else \
    {
      set from_pins [ list_objects [ get_pins "$inst${from_pin}" ] ]
      set to_pins   [ list_objects [ get_pins "$inst${to_pin}" ] ]
      if { ([llength $from_pins] == 1) && ([llength $to_pins] == 1)} then \
      {
        set_false_path -from [ get_pins ${from_pins} ] \
                       -to   [ get_pins ${to_pins} ]
      }
    }
  }
}

#puts [list_false_paths]


save
set has_violations {C:\Users\Administrator\Desktop\10_27_v04workplace\v04workplace\designer\test_4463andcpuopration\test_4463andcpuopration_pre_layout_has_violations}
set fp [open $has_violations w]
puts $fp [has_violations -short]
close $fp
report -type combinational_loops -format xml {C:\Users\Administrator\Desktop\10_27_v04workplace\v04workplace\designer\test_4463andcpuopration\test_4463andcpuopration_combinational_loops.xml}
if { [catch "file delete -force -- {C:\Users\Administrator\Desktop\10_27_v04workplace\v04workplace\designer\test_4463andcpuopration\pinslacks.txt}"] } {
   ;
}
report -type slack {C:\Users\Administrator\Desktop\10_27_v04workplace\v04workplace\designer\test_4463andcpuopration\pinslacks.txt}
