FTN=gfortran
CHPL=chpl

FTN_OPTS=-fno-underscoring

testCallChapel: testCallChapel.o lib/libchapelProcs.a
	$(FTN) testCallChapel.o -Llib -lchapelProcs `$(CHPL_HOME)/util/config/compileline --libraries` -o testCallChapel $(FTN_OPTS)

lib/libchapelProcs.a: chapelProcs.chpl
	$(CHPL) --library chapelProcs.chpl --ccflags=-Wno-incompatible-pointer-types

testCallChapel.o: testCallChapel.f90
	$(FTN) -c testCallChapel.f90 $(FTN_OPTS)

clean:
	rm -rf lib testCallChapel testCallChapel.o chapelinterface.mod
