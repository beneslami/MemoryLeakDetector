#!/bin/sh

x=1
while [ $x -le 1000 ]
do
	./exe
        x=$(( $x + 1 ))
done
