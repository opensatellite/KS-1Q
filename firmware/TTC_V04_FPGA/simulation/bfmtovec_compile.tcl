### script to compile Actel AMBA BFM source file(s) into vector file(s)
### for simulation
# 05Feb10		Production Release Version 3.0
quietly set chmod_exe    "/bin/chmod"
quietly set linux_exe    "./bfmtovec.lin"
quietly set windows_exe  "./bfmtovec.exe"
quietly set bfm_in1      "./coreapb3_usertb_master.bfm"
quietly set bfm_out1     "./coreapb3_usertb_master.vec"
quietly set log          "./bfmtovec_compile.log"

# check OS type and use appropriate executable
if {$tcl_platform(os) == "Linux"} {
	echo "--- Using Linux Actel DirectCore AMBA BFM compiler"
	quietly set bfmtovec_exe $linux_exe
	if {![file executable $bfmtovec_exe]} {
		quietly set cmds "exec $chmod_exe +x $bfmtovec_exe"
		eval $cmds
	}
} else {
	echo "--- Using Windows Actel DirectCore AMBA BFM compiler"
	quietly set bfmtovec_exe $windows_exe
}
# compile BFM source file(s) into vector output file(s)
echo "--- Compiling Actel DirectCore AMBA BFM source files ..."
quietly set cmd1 "exec $bfmtovec_exe -in $bfm_in1 -out $bfm_out1 > $log"
eval $cmd1

# print contents of log file
quietly set f [open $log]
while {[gets $f line] >= 0} {puts $line}
close $f

echo "--- Done Compiling Actel DirectCore AMBA BFM source files."
