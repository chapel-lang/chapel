include Makefile.chpl_lib

mytest: mytest.c libgetMakefile.a
	@$(CHPL_COMPILER) $(CHPL_CFLAGS) -o mytest mytest.c $(CHPL_LDFLAGS)
	@./mytest
