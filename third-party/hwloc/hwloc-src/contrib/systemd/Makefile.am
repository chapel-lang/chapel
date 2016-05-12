# Copyright © 2016 Inria.  All rights reserved.
# See COPYING in top-level directory.

# We can't use autoconf because we don't want $sbindir to be replaced with ${exec_prefix}/sbin
# Also don't install since , let the packagers do it.

EXTRA_DIST = hwloc-dump-hwdata.service.in

if HWLOC_HAVE_LINUX
nodist_pkgdata_DATA = hwloc-dump-hwdata.service
# depend on Makefile in case we rerun configure with different directory arguments
hwloc-dump-hwdata.service: hwloc-dump-hwdata.service.in Makefile
	@echo Creating $@...
	@ $(SED) \
	  -e 's@#SBINDIR#@'$(sbindir)'@g' \
	  -e 's@#RUNSTATEDIR#@'$(HWLOC_runstatedir)'@g' \
	  -e 's@#RMPATH#@'$(RMPATH)'@g' \
	  > $@ < $<

distclean-local:
	rm -f hwloc-dump-hwdata.service
endif
