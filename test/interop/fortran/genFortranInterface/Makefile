CHPL=chpl
MODULENAME=chapelProcs
MODULENAME_LOWER=$(shell echo $(MODULENAME) | tr '[:upper:]' '[:lower:]')
FTN=gfortran

$(MODULENAME): $(MODULENAME_LOWER).mod lib/libchapelProcs.a testCallChapel.f90
	$(FTN) testCallChapel.f90 -Llib -l$(MODULENAME) `$(CHPL_HOME)/util/config/compileline --libraries` -o chapelProcs -fno-underscoring

$(MODULENAME_LOWER).mod: lib/$(MODULENAME).f90
	$(FTN) -c -fno-underscoring lib/$(MODULENAME).f90

lib/$(MODULENAME).f90: chapelProcs.chpl
	$(CHPL) chapelProcs.chpl --library-fortran-name=$(MODULENAME) --ccflags=-Wno-incompatible-pointer-types

lib/libchapelProcs.a: lib/$(MODULENAME).f90

clean:
	rm -rf $(MODULENAME) $(MODULENAME_LOWER).mod lib/ $(MODULENAME).o
