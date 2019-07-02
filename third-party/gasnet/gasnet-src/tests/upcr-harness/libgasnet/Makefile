all: 
	@echo This Makefile is not intended to be called directly.
	@echo It is used for running unit tests within the automated testing harness provided with the Berkeley UPC runtime.
	@echo For information on downloading and using Berkeley UPC, see: https://upc.lbl.gov
	@exit 1

PERL ?= perl
LIBGASNET_LOGDIR ?= $(HARNESS_LOGDIR)
LIBGASNET_CONFIGURE ?= configure# can also be cross-configure-*
LIBGASNET_CONFIGURE_ARGS ?=$(shell $(PERL) -ne 'if (m/^\s*CONFIGURE_ARGS\s*=\s*(.*)$$/) { print $$1; }' < $(TOP_BUILDDIR)/gasnet/Makefile )# configure args
LIBGASNET_CONFIGURE_ARGS_EXTRA ?=# extra configure args to append
LIBGASNET_CONFIGURE_ENV ?=# envvars for configure
SUITE_BUILD_DIR:=$(shell pwd)/bgasnet

_configure: force
	@rm -f $@
	rm -Rf $(SUITE_BUILD_DIR)
	mkdir $(SUITE_BUILD_DIR)
	@logfile=$(LIBGASNET_LOGDIR)/libgasnet-configure.log ; \
	 configure=$(TOP_SRCDIR)/gasnet/$(LIBGASNET_CONFIGURE) ; \
	 crossconfig=$(TOP_SRCDIR)/gasnet/other/contrib/$(LIBGASNET_CONFIGURE) ; \
	 if ! test -x $$configure && test -x $$crossconfig ; then \
	   ln -s $$crossconfig $$configure ; \
	 fi ; \
	 set -x ; \
	 ( cd $(SUITE_BUILD_DIR) && \
	   env $(LIBGASNET_CONFIGURE_ENV) \
	   $$configure $(LIBGASNET_CONFIGURE_ARGS) $(LIBGASNET_CONFIGURE_ARGS_EXTRA) ) > $$logfile 2>&1 ; \
	 rc=$$? ; cat $$logfile ; exit $$rc
	@echo '#!/bin/sh' > $@ ; chmod +x $@

do-preclean-mpi: force
	$(MAKE) -C $(SUITE_BUILD_DIR)/other/ammpi clean

do-preclean-udp: force
	$(MAKE) -C $(SUITE_BUILD_DIR)/other/amudp clean

do-preclean-%: force
	echo $@

do-clean: force
	$(MAKE) do-preclean-$(NETWORK)
	$(MAKE) -C $(SUITE_BUILD_DIR)/$(NETWORK)-conduit clean

do-conduit: force
	$(MAKE) do-clean
	$(MAKE) -C $(SUITE_BUILD_DIR)/$(NETWORK)-conduit $(MAKE_TARGET)
	$(MAKE) do-clean

LIBGASNET_PEDANTIC_WOFF ?=
LIBGASNET_PEDANTIC_WOFF_C ?=
LIBGASNET_PEDANTIC_WOFF_CXX ?=
LIBGASNET_PEDANTIC_FLAGS ?= \
  MANUAL_CFLAGS="-std=c99 -pedantic $(LIBGASNET_PEDANTIC_WOFF) $(LIBGASNET_PEDANTIC_WOFF_C)" \
  MANUAL_CXXFLAGS="-std=c++11 -pedantic $(LIBGASNET_PEDANTIC_WOFF) $(LIBGASNET_PEDANTIC_WOFF_CXX)"

pedantic-%: $(SUITE_BUILD_DIR) force
	@rm -f $@
	@network=`echo $@ | $(PERL) -pe 's/^(.*)-(.*)-(.*)$$/$$2/'`; \
	 target=`echo $@ | $(PERL) -pe 's/^(.*)-(.*)-(.*)$$/$$3/'`; \
	 logfile=$(LIBGASNET_LOGDIR)/libgasnet-$@.log ; \
	 set -x ; \
	 ( $(MAKE) do-conduit NETWORK=$$network MAKE_TARGET=$$target $(LIBGASNET_PEDANTIC_FLAGS) ) > $$logfile 2>&1 ; \
	 rc=$$? ; cat $$logfile ; exit $$rc
	@echo '#!/bin/sh' > $@ ; chmod +x $@
	 
force:

.PHONY: force

