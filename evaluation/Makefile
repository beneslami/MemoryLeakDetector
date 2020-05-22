TARGET: obj

obj:
	gcc -g -c ../src/mld.c -o mld.o
	gcc -g -c app.c -o app_mld.o
	gcc -g -c app2.c -o app_nrm.o
	gcc -g -o exe_mld mld.o app_mld.o
	gcc -g -o exe_nrm mld.o app_nrm.o

clean:
	rm -rf app.o mld.o app_mld.o app_nrm.o exe_mld exe_nrm exe
