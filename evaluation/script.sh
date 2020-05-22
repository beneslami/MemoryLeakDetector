#!/bin/bash

x=1
while [ $x -le 1000 ]
do
	(perf stat -x' ' ./exe_mld) 2>&1 | cut -d' ' -f1 | awk '/^[0-9]/ { printf "%s", $1; }' >> mld_out.txt
	(perf stat -x' ' ./exe_nrm) 2>&1 | cut -d' ' -f1 | awk '/^[0-9]/ { printf "%s", $1; }' >> nrm_out.txt
	echo '\n' >> mld_out.txt
	echo '\n' >> nrm_out.txt 
        x=$(( $x + 1 ))
done
