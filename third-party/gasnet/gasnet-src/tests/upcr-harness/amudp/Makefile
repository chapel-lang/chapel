all: 
	@echo This Makefile is not intended to be called directly.
	@echo It is used for running unit tests within the automated testing harness provided with the Berkeley UPC runtime.
	@echo For information on downloading and using Berkeley UPC, see: https://upc.lbl.gov
	@exit 1

PERL ?= perl
AMX_FLAVOR=udp
CONDUIT_DIR=$(TOP_BUILDDIR)/gasnet/$(AMX_FLAVOR)-conduit
CONDUIT_FRAG=$(CONDUIT_DIR)/$(AMX_FLAVOR)-seq.mak
AMX_DIR=$(TOP_BUILDDIR)/gasnet/other/am$(AMX_FLAVOR)

Makefile: $(CONDUIT_FRAG)
$(CONDUIT_FRAG):
	@if test -d $(CONDUIT_DIR) ; then                                \
	   $(MAKE) -C $(CONDUIT_DIR) $(@F) || exit 1                     \
	 else                                                            \
	   echo ERROR: $(AMX_FLAVOR)-conduit build directory not found ; \
	 fi
-include $(CONDUIT_DIR)/$(AMX_FLAVOR)-seq.mak

amxdist: force
PASSTHRU_PHONY=check check-exports tests amxdist

$(PASSTHRU_PHONY): force
	$(MAKE) -C $(AMX_DIR) $@
	@echo '#!/bin/sh' > $@ ; chmod +x $@

distcheck: force
	rm -Rf am$(AMX_FLAVOR)[0-9].[0-9]*
	$(MAKE) -C $(AMX_DIR) amxdist
	@set -x ; \
	 TESTDIR=`pwd` ; \
	 cd $(AMX_DIR) && \
	 VERSION=`/bin/ls -t am$(AMX_FLAVOR)*.tar.gz | $(PERL) -ne 'if (m@am$(AMX_FLAVOR)(.+)\.tar.gz@) { print "$$1"; exit; }'` ; \
	 DIR="am$(AMX_FLAVOR)$$VERSION" ; \
	 gunzip -c $$DIR.tar.gz | tar xvf - && \
	 cd $$DIR && \
	 $(MAKE) all tests CC="$(GASNET_CC)" CXX="$(GASNET_LD)" && \
	 cp ./amudprun$(EXESUFFIX) $$TESTDIR/amudprun-sa$(EXESUFFIX) && \
	 cp ./testam$(EXESUFFIX) $$TESTDIR/$@$(EXESUFFIX)

amudprun:
	$(MAKE) -C $(AMX_DIR) $@$(EXESUFFIX)
	@cp $(AMX_DIR)/$@$(EXESUFFIX) .

# This target tests the Makefile target that builds amudprun for the host system 
# When cross-compiling, this requries access to host compilers (which differ from the usual target compilers)
# UPCR and harness have proper support for host C compilers, but unfortunately not host C++ compilers
# For non-cross-compile, we default these to GASNet's target C++ compiler
# For cross-compile, the user should set HOST_CXX(FLAGS) in the harness environment
# We deliberately ban the MANUAL_* flags here, as they may not be appropriate for the host compiler
# note this "banning" is only effective for cross-compile, otherwise MANUAL_CFLAGS is already folded into HOST_CFLAGS by harness
amudprun-host:
	rm -Rf $(TOP_BUILDDIR)/gasnet/other/amudp/.host
	@echo "Harness default settings:" 1>&2 ; \
	 set -x ; HOST_CC='$(HOST_CC)' HOST_CFLAGS='$(HOST_CFLAGS)' HOST_CXX='$(HOST_CXX)' HOST_CXXFLAGS='$(HOST_CXXFLAGS)' HOST_LDFLAGS='$(HOST_LDFLAGS)' HOST_LIBS='$(HOST_LIBS)' ; set +x ; \
	 if test -z "$$HOST_CXX" ; then \
	   echo "Defaulting HOST_CXX(FLAGS) to GASNET_CXX(FLAGS)" 1>&2 ; set -x ; \
	   eval HOST_`$(MAKE) --no-print-directory -f $(TOP_BUILDDIR)/gasnet/Makefile echovar VARNAME=CXX` ; \
	   eval HOST_`$(MAKE) --no-print-directory -f $(TOP_BUILDDIR)/gasnet/Makefile echovar VARNAME=CXXFLAGS` ; \
	 fi ; \
	 set -x ; \
	 $(MAKE) -C $(AMX_DIR) $@$(EXESUFFIX) CC="$$HOST_CC $$HOST_CFLAGS" CXX="$$HOST_CXX $$HOST_CXXFLAGS" platform_ldflags="$$HOST_LDFLAGS $$HOST_LIBS" \
	                                      MANUAL_CFLAGS='' MANUAL_CXXFLAGS='' MANUAL_DEFINES='' MANUAL_LIBS='' MANUAL_LDFLAGS=''
	@cp $(AMX_DIR)/$@$(EXESUFFIX) .

test%: amudprun force
	$(MAKE) -C $(AMX_DIR) $@
	@cp $(AMX_DIR)/$@$(EXESUFFIX) .

force:

.PHONY: force

