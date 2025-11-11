#!/bin/bash

No_of_run=5
No_of_events=100
Input_filename="Generator_output*"
Output_filename="N_Sim_output_P"

rm -f Run_output*  output.log

i=1
while [ $i -le $No_of_run ] 
do
	echo "=== Starting run $i ===" >> output.log
	./g4cmpPhonon ./run.mac >> output.log 2>&1
	if [ $? -ne 0 ]; then
		echo "Run $i failed" >> output.log
		break
	fi

	# Merge ROOT files
    	hadd -f "${No_of_events}_${Output_filename}${i}.root" ${Input_filename} >> output.log 2>&1
    	if [ $? -ne 0 ]; then
       		 echo "hadd failed for run $i" >> output.log
       	 	break
    	fi

    	# Clean up intermediate outputs
    	rm -f Generator_output*
	
	((i++))
done
