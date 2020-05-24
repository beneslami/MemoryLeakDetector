import sys, string, os, time
from memprof import memprof

@profile
def mld():
	os.popen("./exe_mld")
@profile
def normal():
	os.popen("./exe_nrm")
@profile
def valgrind():
	os.popen("valgrind ./exe_nrm")

if __name__ == '__main__':
	mld()
	time.sleep(0.5)
	normal()
	time.sleep(0.5)
	valgrind()
