# open the project, don't forget to reset
open_project -reset met_testf2
set_top MET_hw
add_files MET.cpp
add_files -tb MET_test.cpp 
add_files -tb MET_ref.cpp

# reset the solution
open_solution -reset "solution_0617"
# part options:
#	xcku9p-ffve900-2-i-EVAL
#	xc7vx690tffg1927-2
#	xcku5p-sfvb784-3-e
#	xcku115-flvf1924-2-i
#	xcvu9p-flga2104-2l-e
set_part {xc7vx690tffg1927-2}
create_clock -period 5 -name default

# do stuff
csim_design
csynth_design
#cosim_design -trace_level all
#export_design -format ip_catalog  -vendor "cern-cms"

# exit Vivado HLS
exit
