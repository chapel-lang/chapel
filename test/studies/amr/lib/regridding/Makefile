MISC_LIB=../misc


Trim_test: Trim_test.out
	./Trim_test.out

Trim_test.out: Trim.chpl
	chpl Trim.chpl -M $(MISC_LIB) -o Trim_test.out

Regrid_test: Regrid_test.out
	./Regrid_test.out

Regrid_test.out: Regrid.chpl
	chpl Regrid.chpl -M $(MISC_LIB) -o Regrid_test.out