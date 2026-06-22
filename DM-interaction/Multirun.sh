#!/bin/bash

No_of_run=200
No_of_events=500

Run_start_offset=0

Input_filename="Simulation_output_*"
Output_filename="N_Sim_output_P"
Start_time=$(date +%s)
rm -f  output.log

i=$(($Run_start_offset+1))
while [ $i -le $No_of_run ] 
do
	echo "=== Starting run $i ===" >> output.log
	G4_USERCONFIG_CUSTOMEVENTID=$(($No_of_events*($i-1))) ./DM-interaction ./run.mac >> output.log 2>&1
	# The below code is commented since the geant4 sim usually produces a harmless segmentation fault
#	if [ $? -ne 0 ]; then
#		echo "Run $i failed" >> output.log
#		break
#	fi

	# Merge ROOT files
    	hadd -f "${No_of_events}_${Output_filename}${i}.root" ${Input_filename} >> output.log 2>&1
    	if [ $? -ne 0 ]; then
       		 echo "hadd failed for run $i" >> output.log
       	 	break
    	fi

    	# Clean up intermediate sim outputs
    	rm -f ${Input_filename}
	
	((i++))
done
End_time=$(date +%s)
echo "Time taken to run $(($No_of_run-$Run_start_offset)) simulations: $((($End_time-$Start_time)/60)) min" >> output.log
