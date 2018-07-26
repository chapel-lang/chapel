ifeq ($(MAKECMDGOALS),mytest)
include Makefile.getMakefile
mytest: mytest.c libgetMakefile.a
	@$(CHPL_COMPILER) $(CHPL_CFLAGS) -o mytest mytest.c $(CHPL_LDFLAGS)
	@./mytest
else
mytest:
	@echo "weird"
endif

ifeq ($(MAKECMDGOALS),renamedTest)
include Makefile.myLib
renamedTest: renamedTest.c myLib.a
	@$(CHPL_COMPILER) $(CHPL_CFLAGS) -o renamedTest renamedTest.c $(CHPL_LDFLAGS)
	@./renamedTest
else
renamedTest:
	@echo "weird"
endif
