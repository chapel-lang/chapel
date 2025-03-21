all: 
	@echo This Makefile is not intended to be called directly.
	@echo It is used for running unit tests within the automated testing harness provided with the Berkeley UPC runtime.
	@echo For information on downloading and using Berkeley UPC, see: https://upc.lbl.gov
	@exit 1

# legacy harness support
UPCR_BLDDIR ?= $(TOP_BUILDDIR)
GASNET_BLDDIR ?= $(TOP_BUILDDIR)/gasnet

PERL ?= perl
CONDUIT_DIR=$(GASNET_BLDDIR)/$(NETWORK)-conduit

testcxx%:       force ; $(MAKE) do-cxx-test EXE=$@
testlegacycxx%: force ; $(MAKE) do-cxx-test EXE=$@

do-cxx-test: force
	@if test -n "`$(UPCC) --network=udp --echo-var UPCR_LD 2>/dev/null`"; then      \
	  makename=`echo $(EXE) | $(PERL) -pe 's/_.*-(seq|par|parsync)$$/-$$1/'`;       \
	   cmdname=`echo $(EXE) | $(PERL) -pe 's/(_.*)?-(seq|par|parsync)$$//'`;        \
	   echo "$$makename => $$cmdname => $(EXE)" ; set -x ;                          \
	   $(MAKE) -C $(CONDUIT_DIR) $$makename MANUAL_CXXFLAGS="$(MANUAL_CXXFLAGS)" && \
	   cp -f $(CONDUIT_DIR)/$$cmdname$(EXESUFFIX) $(EXE)$(EXESUFFIX);               \
	 else                                                                           \
	   echo '#!/bin/sh' > $(EXE) ; chmod +x $(EXE);                                 \
	 fi

# One rule to rule them all
# test(.*)(_.*)?-(seq|par|parsync) gets made as test$1-$3 into executable test$1 and copied to $0
test%: force
	@makename=`echo $@ | $(PERL) -pe 's/_.*-(seq|par|parsync)$$/-$$1/'`; \
	 cmdname=`echo $@ | $(PERL) -pe 's/(_.*)?-(seq|par|parsync)$$//'`; \
	 echo "$$makename => $$cmdname => $@" ; set -x ; \
         $(MAKE) -C $(CONDUIT_DIR) $$makename MANUAL_CFLAGS="$(MANUAL_CFLAGS)" && \
         cp -f $(CONDUIT_DIR)/$$cmdname$(EXESUFFIX) $@$(EXESUFFIX)

testconduitspecific: force
	@if test "$(NETWORK)" = "mpi" ; then 	\
           conduit_test=testmpi ;		\
           conduit_mode=par ;			\
         fi ;					\
	 if test "$(NETWORK)" = "udp" ; then 	\
           conduit_test=testcxx ;		\
           conduit_mode=par ;			\
         fi ;					\
	 if test "$$conduit_test" = "" ; then 	\
           conduit_test=testam ;		\
           conduit_mode=parsync ;		\
         fi ;					\
         $(MAKE) -C $(CONDUIT_DIR) $$conduit_test-$$conduit_mode MANUAL_CFLAGS="$(MANUAL_CFLAGS)" && \
	 cp -f $(CONDUIT_DIR)/$$conduit_test$(EXESUFFIX) $@$(EXESUFFIX)

parsync: force
	@$(MAKE) -C $(CONDUIT_DIR) $@
	@echo '#!/bin/sh' > $@ ; chmod +x $@

check-exports: force
	@$(MAKE) -C $(CONDUIT_DIR) $@
	@echo '#!/bin/sh' > $@ ; chmod +x $@

check-tools-exports: force
	@$(MAKE) -C $(GASNET_BLDDIR) check-exports
	@echo '#!/bin/sh' > $@ ; chmod +x $@

check-libupcr-exports: force
	@$(MAKE) -C $(UPCR_BLDDIR) check-exports
	@echo '#!/bin/sh' > $@ ; chmod +x $@

do-check-other: force
	@rm -f $(GASNET_BLDDIR)/other/check-pp-tgt$(EXESUFFIX)
	@$(MAKE) -C $(GASNET_BLDDIR)/other check-local
	@cp -f $(GASNET_BLDDIR)/other/check-pp-tgt$(EXESUFFIX) $(EXE)$(EXESUFFIX)

check-other:          force ; $(MAKE) do-check-other EXE=$@
check-other-cross:    force ; $(MAKE) do-check-other EXE=$@
check-other-pedantic: force ; $(MAKE) do-check-other EXE=$@

force:

.PHONY: force

