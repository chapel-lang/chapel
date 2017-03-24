ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

export CHPL_MAKE_LOCALE_MODEL

#
# set up the directories
#
QTHREAD_ABS_DIR = $(shell pwd)
QTHREAD_INSTALL_DIR = $(QTHREAD_ABS_DIR)/$(QTHREAD_INSTALL_SUBDIR)
QTHREAD_BUILD_DIR = $(QTHREAD_ABS_DIR)/$(QTHREAD_BUILD_SUBDIR)
QTHREAD_DIR = $(QTHREAD_ABS_DIR)

#
# Cray X* builds are cross-compilations
#
ifneq (, $(filter cray-x%,$(CHPL_MAKE_TARGET_PLATFORM)))
CHPL_QTHREAD_CFG_OPTIONS += --host=x86_64-cle-linux-gnu
else ifneq ($(CHPL_MAKE_HOST_PLATFORM),$(CHPL_MAKE_TARGET_PLATFORM))
CHPL_QTHREAD_CFG_OPTIONS += --host=$(CHPL_MAKE_TARGET_PLATFORM)-unknown-linux-gnu
endif

ifeq ($(CHPL_MAKE_HWLOC),hwloc)
CHPL_QTHREAD_CFG_OPTIONS += --with-topology=hwloc --with-hwloc=$(HWLOC_INSTALL_DIR)
endif

# Have qthreads use Chapel's allocator, unless directed not to
ifeq (, $(call isTrue, $(CHPL_QTHREAD_NO_CHPL_ALLOC)))
CHPL_QTHREAD_CFG_OPTIONS += --with-alloc=chapel
CFLAGS += -I$(CHPL_MAKE_HOME)/runtime/include/mem/$(CHPL_MAKE_MEM)/
CFLAGS += -I$(CHPL_MAKE_HOME)/runtime/include/
ifeq ($(CHPL_MAKE_MEM),jemalloc)
CFLAGS += -I$(JEMALLOC_INCLUDE_DIR)
RUNTIME_LFLAGS += -L$(JEMALLOC_LIB_DIR)
RUNTIME_LFLAGS += -ljemalloc
RUNTIME_LFLAGS += $(shell $(JEMALLOC_BIN_DIR)/jemalloc-config --libs)
endif
endif

# enable oversubscription for testing
ifneq (, $(CHPL_QTHREAD_ENABLE_OVERSUBSCRIPTION))
CHPL_QTHREAD_CFG_OPTIONS += --enable-oversubscription
endif

# enable guard pages for stack overflow detection, unless directed not to
ifeq (, $(call isTrue, $(CHPL_QTHREAD_NO_GUARD_PAGES)))
CHPL_QTHREAD_CFG_OPTIONS += --enable-guard-pages
endif

# optionally enable printing compilation commands, etc. (to help diagnose
# build problems)
ifneq (, $(CHPL_QTHREAD_LOUD_RULES))
CHPL_QTHREAD_CFG_OPTIONS += --disable-silent-rules
endif

CHPL_QTHREAD_CFG_OPTIONS += --enable-static --disable-shared

# determine which scheduler to use. Default to nemesis, except for numa where
# we want distrib. Override with a user provided option if they requested one
SCHEDULER = nemesis
ifneq ($(CHPL_MAKE_LOCALE_MODEL),flat)
SCHEDULER = distrib
endif
ifneq (, $(CHPL_QTHREAD_SCHEDULER))
SCHEDULER = $(CHPL_QTHREAD_SCHEDULER)
endif
CHPL_QTHREAD_CFG_OPTIONS += --with-scheduler=$(SCHEDULER)

# spawn-caching has a semantic mismatch with chapel (and leads to deadlock with
# some applications.) Qthreads team tends to build with spawn cache off too
CHPL_QTHREAD_CFG_OPTIONS += --disable-spawn-cache

# reduce performance penalty in cases where numChapelTasks < numQthreadWorkers
CHPL_QTHREAD_CFG_OPTIONS += --enable-condwait-queue

# pgi doesn't support the proper atomic intrinsics that are required for
# spinlocks to be fast, so disable them for pgi, and PrgEnv-pgi
ifneq (, $(findstring pgi,$(CHPL_MAKE_TARGET_COMPILER)))
CHPL_QTHREAD_CFG_OPTIONS += --disable-internal-spinlock
endif

CHPL_QTHREAD_CFG_OPTIONS += $(CHPL_QTHREAD_MORE_CFG_OPTIONS)

default: all

all: qthread

clean: FORCE
	rm -rf $(QTHREAD_BUILD_SUBDIR)

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install


qthread-config: FORCE
#
# These first few lines touch a bunch of autoconf-oriented files in a
# certain order to prevent autoconf from running again; otherwise, we
# ran into issues if a user's autoconf environment was not as far
# ahead in version numbers as that which was used when packaging the
# Qthreads release
#
	cd $(QTHREAD_SUBDIR) && touch -c configure.ac
	cd $(QTHREAD_SUBDIR) && find . -name "*.m4" | xargs touch
	cd $(QTHREAD_SUBDIR) && touch -c aclocal.m4
	cd $(QTHREAD_SUBDIR) && touch configure
	cd $(QTHREAD_SUBDIR) && find . -name "*.in" | xargs touch

#
# Then configure
#
	mkdir -p $(QTHREAD_BUILD_DIR)
	cd $(QTHREAD_BUILD_DIR) \
	&& $(QTHREAD_SUBDIR)/configure CC='$(CC)' CFLAGS='$(CFLAGS)' \
	       CXX='$(CXX)'  CXXFLAGS='$(CFLAGS)' LDFLAGS='$(RUNTIME_LFLAGS)' \
	       --prefix=$(QTHREAD_INSTALL_DIR) $(CHPL_QTHREAD_CFG_OPTIONS)

qthread-build: FORCE
	cd $(QTHREAD_BUILD_DIR) && $(MAKE)
	cd $(QTHREAD_BUILD_DIR) && $(MAKE) install

#
# The two variables here answer different questions even though they're
# set using the same logic.  For the first, the question is "Will remote
# caching work with this Qthreads build?", which is true iff qthreads do
# not move from one worker to another (thus invalidating TLS).  For the
# second, it's "Is there only one worker per shepherd?", which changes
# how the shim sets certain QT_* environment variables to parameterize
# Qthreads behavior.  These two questions have the same scheduler-based
# answer now, but that may not always be true.  If and when it's not,
# we'll need separate checks to set them.
#
ifeq ($(SCHEDULER),$(findstring $(SCHEDULER),lifo mtsfifo mutexfifo nemesis))
SUPPORTS_REMOTE_CACHE = 1
ONE_WORKER_PER_SHEPHERD = 1
else ifeq ($(SCHEDULER),$(findstring $(SCHEDULER),distrib nottingham sherwood))
SUPPORTS_REMOTE_CACHE = 0
ONE_WORKER_PER_SHEPHERD = 0
else
$(error Unrecognized Qthreads scheduler '$(SCHEDULER)')
endif

qthread-chapel-h: FORCE
	echo "#define CHPL_QTHREAD_SUPPORTS_REMOTE_CACHE" \
	     $(SUPPORTS_REMOTE_CACHE) \
	     > $(QTHREAD_INSTALL_DIR)/include/qthread-chapel.h
	echo "#define CHPL_QTHREAD_SCHEDULER_ONE_WORKER_PER_SHEPHERD" \
	     $(ONE_WORKER_PER_SHEPHERD) \
	     >> $(QTHREAD_INSTALL_DIR)/include/qthread-chapel.h

qthread: qthread-config qthread-build qthread-chapel-h

qthread-reconfig:
	cd $(QTHREAD_SUBDIR) && autoreconf -f -i

FORCE:

.NOTPARALLEL:
