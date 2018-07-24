include Makefile.chpl_lib

mytest: mytest.c libgetMakefile.a
	@$(CHPL_COMPILER) $(CHPL_CFLAGS) -o mytest $(CHPL_LDFLAGS) mytest.c
	@./mytest
