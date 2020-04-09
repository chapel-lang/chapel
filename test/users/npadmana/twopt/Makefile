all :
	chpl -O --fast -o do_smu_aos.x do_smu_aos.chpl
	chpl -O --fast -o do_smu_soa.x do_smu_soa.chpl

optimize :
	chpl -O --fast --no-specialize --savec c --ccflags -g -o do_smu_aos.x -lprofiler do_smu_aos.chpl

clean :
	-rm -rf *.x c/* *.prof
	-rm test-DD.dat.{brute,tree}

