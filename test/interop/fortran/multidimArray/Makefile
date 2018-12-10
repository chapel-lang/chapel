CHPL=chpl
FTN=ifort
MODULENAME=chapelProcs
MODULENAME_LOWER=$(shell echo $(MODULENAME) | tr '[:upper:]' '[:lower:]')
# disable incompatible pointer warnings
WARNINGS=-wd167
UNDERSCORES=
#UNDERSCORES=-fno-underscoring

$(MODULENAME): $(MODULENAME_LOWER).mod lib/libchapelProcs.a passArrayToChapel.f90
	$(FTN) passArrayToChapel.f90 -Llib -l$(MODULENAME) `$(CHPL_HOME)/util/config/compileline --libraries` -o chapelProcs $(UNDERSCORES)

$(MODULENAME_LOWER).mod: lib/$(MODULENAME).f90
	$(FTN) -c $(UNDERSCORES) lib/$(MODULENAME).f90

lib/$(MODULENAME).f90: chapelProcs.chpl
	$(CHPL) chapelProcs.chpl --library-fortran --ccflags=$(WARNINGS)

lib/libchapelProcs.a: lib/$(MODULENAME).f90

clean:
	rm -rf $(MODULENAME) $(MODULENAME_LOWER).mod lib/ $(MODULENAME).o
