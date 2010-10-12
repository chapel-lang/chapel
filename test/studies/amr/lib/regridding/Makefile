MISC_LIB=../misc
GRID_LIB=../grid
LEVEL_LIB=../level
AMR_LIB=../amr


Regridder_test: Regridder_test.out
	./Regridder_test.out

Regridder_test.out: Regridder_def.chpl FlaggedDomain_def.chpl Stack_def.chpl
	chpl Regridder_def.chpl FlaggedDomain_def.chpl Stack_def.chpl --main-module=Regridder_def -M $(MISC_LIB) -M $(GRID_LIB) -M $(LEVEL_LIB) -M $(AMR_LIB) -o Regridder_test.out


FlaggedDomainSet_test: FlaggedDomainSet_test.out
	./FlaggedDomainSet_test.out

FlaggedDomainSet_test.out: FlaggedDomainSet_def.chpl Stack_def.chpl
	chpl FlaggedDomainSet_def.chpl Stack_def.chpl --main-module=FlaggedDomainSet_def -M $(MISC_LIB) -o FlaggedDomainSet_test.out
