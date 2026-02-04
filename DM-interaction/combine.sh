#!/bin/bash

# Code to combine the output files



rm Generator-output.txt phonon_hits.txt

for n in {0..29..1};		# {<start=0>,<threads-1>,<step=1>}
do
	echo "Copying file no. $n"
	cat Generator_output_G4WT$n.txt >> Generator-output.txt
	cat phonon_hits_G4WT$n.txt >> phonon_hits.txt
done
