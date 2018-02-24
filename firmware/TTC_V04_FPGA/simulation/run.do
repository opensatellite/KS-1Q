quietly set ACTELLIBNAME SmartFusion2
quietly set PROJECT_DIR "C:/Users/Administrator/Desktop/workplace"
source "${PROJECT_DIR}/simulation/CM3_compile_bfm.tcl";source "${PROJECT_DIR}/simulation/bfmtovec_compile.tcl";

if {[file exists ../designer/test_4463andcpuopration/simulation/postlayout/_info]} {
   echo "INFO: Simulation library ../designer/test_4463andcpuopration/simulation/postlayout already exists"
} else {
   file delete -force ../designer/test_4463andcpuopration/simulation/postlayout 
   vlib ../designer/test_4463andcpuopration/simulation/postlayout
}
vmap postlayout ../designer/test_4463andcpuopration/simulation/postlayout
vmap SmartFusion2 "G:/Microsemi/Libero_SoC_v11.7/Designer/lib/modelsim/precompiled/vlog/SmartFusion2"
if {[file exists CORERSENC_LIB/_info]} {
   echo "INFO: Simulation library CORERSENC_LIB already exists"
} else {
   file delete -force CORERSENC_LIB 
   vlib CORERSENC_LIB
}
vmap CORERSENC_LIB "CORERSENC_LIB"
if {[file exists COREAPB3_LIB/_info]} {
   echo "INFO: Simulation library COREAPB3_LIB already exists"
} else {
   file delete -force COREAPB3_LIB 
   vlib COREAPB3_LIB
}
vmap COREAPB3_LIB "COREAPB3_LIB"

vlog  -work postlayout "${PROJECT_DIR}/designer/test_4463andcpuopration/test_4463andcpuopration_ba.v"

vsim -L SmartFusion2 -L postlayout -L CORERSENC_LIB -L COREAPB3_LIB  -t 1fs -sdfmax /test_4463andcpuopration=${PROJECT_DIR}/designer/test_4463andcpuopration/test_4463andcpuopration_ba.sdf postlayout.test_4463andcpuopration
# The following lines are commented because no testbench is associated with the project
# add wave /testbench/*
# run 1000ns
