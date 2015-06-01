all :
	chpl -O --fast -o do_smu.x do_smu.chpl
	chpl -O --fast -o do_smu_soa.x do_smu_soa.chpl

nospec :
	chpl -O --fast --no-specialize -o do_smu.x do_smu.chpl
	chpl -O --fast --no-specialize -o do_smu_soa.x do_smu_soa.chpl


optimize :
	chpl -O --fast --no-specialize --savec c --ccflags -g -o do_smu.x -lprofiler do_smu.chpl

clean :
	-rm -rf *.x c/* *.prof
	-rm test-DD.dat.{brute,tree}

