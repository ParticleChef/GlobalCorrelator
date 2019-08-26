# open the project, don't forget to reset
open_project -reset met
set_top SIN_hw
add_files SIN.cpp
add_files -tb SIN_test.cpp 

# reset the solution
open_solution -reset "sin_test"
# part options:
#	xcku9p-ffve900-2-i-EVAL
#	xc7vx690tffg1927-2
#	xcku5p-sfvb784-3-e
#	xcku115-flvf1924-2-i
#set_part {xc7vx690tffg1927-2}
set_part {xcvu9p-flga2104-2l-e}
create_clock -period 5 -name default

# do stuff
csim_design
csynth_design
#cosim_design -trace_level all
#export_design -format ip_catalog  -vendor "cern-cms"

# exit Vivado HLS
exit
