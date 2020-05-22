#!/bin/bash

x=1
while [ $x -le 1000 ]
do
	perf stat -e task-clock -o temp2.txt ./exe_nrm
	sed -n 6p temp2.txt | grep -Eo "[0-9][0-9]*\.[0-9]*  " >> perf_nrm.txt
	perf stat -e task-clock -o temp.txt valgrind ./exe_nrm
	sed -n 6p temp.txt | grep -Eo "[0-9][0-9]*\.[0-9]*  " >> perf_mld.txt
        x=$(( $x + 1 ))
done
