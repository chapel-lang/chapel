ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --host
include $(CHPL_MAKE_HOME)/make/Makefile.base

platform_defines = -DAMUDP_ENV_PREFIX=GASNET -Dif_pf=if -Dif_pt=if
#
# The amudprun launcher is to be run on the host, rather than the
# target.  The following test of the environment should really be done
# by a configure script, but it cannot take advantage of the output of
# the target's configure, and it cannot reuse that script because it
# wants all of GASNet.  Here we just want to use amudprun's
# Makefile.standalone.
#
# A better way to do this is for the GASNet distribution itself to
# compile launchers for the host instead of the target, rather than
# doing that here.
#
ifneq (,$(shell grep getifaddrs /usr/include/ifaddrs.h 2>/dev/null))
platform_defines += -DHAVE_GETIFADDRS -DHAVE_IFADDRS_H
endif

default: all

all: amudprun

clean: FORCE
	rm -rf $(AMUDPRUN_BUILD_SUBDIR)

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install

amudprun: configure-amudprun build-amudprun install-amudprun

configure-amudprun: FORCE
	mkdir -p $(AMUDPRUN_BUILD_DIR)
	cp $(AMUDPRUN_SRC_DIR)/Makefile* $(AMUDPRUN_BUILD_DIR)
	cp $(AMUDPRUN_SRC_DIR)/*.c $(AMUDPRUN_BUILD_DIR)
	cp $(AMUDPRUN_SRC_DIR)/*.cpp $(AMUDPRUN_BUILD_DIR)
	cp $(AMUDPRUN_SRC_DIR)/*.h $(AMUDPRUN_BUILD_DIR)
	cp $(AMUDPRUN_SRC_DIR)/../portable_*.h $(AMUDPRUN_BUILD_DIR)

build-amudprun: FORCE
	cd $(AMUDPRUN_BUILD_DIR) && $(MAKE) -f Makefile.standalone CC="$(CC)" CXX="$(CXX)" platform_defines="$(platform_defines)"

install-amudprun: FORCE
	mkdir -p $(AMUDPRUN_INSTALL_DIR)/bin
	cp $(AMUDPRUN_BUILD_DIR)/amudprun $(AMUDPRUN_INSTALL_DIR)/bin


FORCE:

.NOTPARALLEL:
