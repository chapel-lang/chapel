ifeq ($(MAKECMDGOALS),envOverrides)
include lib/Makefile.checkEnvOverrides
envOverrides: envOverrides.c lib/libcheckEnvOverrides.a
	@$(CHPL_COMPILER) $(CHPL_CFLAGS) -o envOverrides envOverrides.c $(CHPL_LDFLAGS)
	@./envOverrides
else
envOverrides:
	@echo "weird"
endif

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
renamedTest: renamedTest.c lib/libmyLib.a
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

ifeq ($(MAKECMDGOALS),checkRequire2)
include lib/Makefile.requireHeader
checkRequire2: checkRequire2.c lib/librequireHeader.a
	@$(CHPL_COMPILER) $(CHPL_CFLAGS) -o checkRequire2 checkRequire2.c $(CHPL_LDFLAGS)
	@./checkRequire2
else
checkRequire2:
	@echo "weird"
endif

ifeq ($(MAKECMDGOALS),checkCommandLineLib)
include lib/Makefile.commandLineLib
checkCommandLineLib: checkCommandLineLib.c lib/libcommandLineLib.a
	@$(CHPL_COMPILER) $(CHPL_CFLAGS) -o checkCommandLineLib checkCommandLineLib.c $(CHPL_LDFLAGS)
	@./checkCommandLineLib
else
checkCommandLineLib:
	@echo "weird"
endif
