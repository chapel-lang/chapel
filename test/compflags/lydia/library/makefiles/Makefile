ifeq ($(MAKECMDGOALS),mytest)
include lib/Makefile.getMakefile
mytest: mytest.c lib/libgetMakefile.a
	@$(CHPL_COMPILER) $(CHPL_CFLAGS) -o mytest mytest.c $(CHPL_LDFLAGS)
	@./mytest
else
mytest:
	@echo "weird"
endif

ifeq ($(MAKECMDGOALS),renamedTest)
include lib/Makefile.myLib
renamedTest: renamedTest.c lib/myLib.a
	@$(CHPL_COMPILER) $(CHPL_CFLAGS) -o renamedTest renamedTest.c $(CHPL_LDFLAGS)
	@./renamedTest
else
renamedTest:
	@echo "weird"
endif

ifeq ($(MAKECMDGOALS),checkRequire)
include lib/Makefile.requireStmt
checkRequire: checkRequire.c lib/librequireStmt.a
	@$(CHPL_COMPILER) $(CHPL_CFLAGS) -o checkRequire checkRequire.c $(CHPL_LDFLAGS)
	@./checkRequire
else
checkRequire:
	@echo "weird"
endif
