optimize :
	chpl -O --fast --no-specialize --savec c --ccflags -g -o do_smu.x -lprofiler do_smu.chpl

clean :
	-rm -rf *.x c/* *.prof
	-rm test-DD.dat.{brute,tree}

