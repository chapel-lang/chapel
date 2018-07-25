include Makefile.chpl_lib

mytest: mytest.c libgetMakefile.a
	@$(CHPL_COMPILER) $(CHPL_CFLAGS) -o mytest mytest.c $(CHPL_LDFLAGS)
	@./mytest

renamedTest: renamedTest.c myLib.a
	@$(CHPL_COMPILER) $(CHPL_CFLAGS) -o renamedTest renamedTest.c $(CHPL_LDFLAGS)
	@./renamedTest
