all: 
	@echo This Makefile is not intended to be called directly.
	@echo It is used for running unit tests within the automated testing harness provided with the Berkeley UPC runtime.
	@echo For information on downloading and using Berkeley UPC, see: http://upc.lbl.gov
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

amudprun amudprun-host:
	$(MAKE) -C $(AMX_DIR) $@$(EXESUFFIX)
	@cp $(AMX_DIR)/$@$(EXESUFFIX) .

test%: amudprun force
	$(MAKE) -C $(AMX_DIR) $@
	@cp $(AMX_DIR)/$@$(EXESUFFIX) .

force:

.PHONY: force

