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
# Cray builds are cross-compilations
#
ifneq (, $(filter cray-x%,$(CHPL_MAKE_TARGET_PLATFORM)))
CHPL_QTHREAD_CFG_OPTIONS += --host=$(CHPL_MAKE_TARGET_ARCH)-cle-linux-gnu
else ifneq (, $(filter cray-cs hpe-cray-ex,$(CHPL_MAKE_TARGET_PLATFORM)))
CHPL_QTHREAD_CFG_OPTIONS += --host=$(CHPL_MAKE_TARGET_ARCH)-unknown-linux-gnu
endif

ifneq ($(CHPL_MAKE_HWLOC),none)
  # Have Qthreads get its hwloc topology from the Chapel runtime,
  # unless directed not to.
  TOPOLOGY=binders
  ifeq (, $(call isTrue, $(CHPL_QTHREAD_DONT_GET_TOPO_FROM_RT)))
    CFLAGS_NEEDS_RT_INCLUDES = y
    CHPL_QTHREAD_CFG_OPTIONS += --with-hwloc-get-topology-function="chpl_topo_getHwlocTopology()"
  endif
  ifeq ($(CHPL_MAKE_HWLOC),bundled)
    CHPL_QTHREAD_CFG_OPTIONS += --with-hwloc=$(HWLOC_INSTALL_DIR)
    ifdef CHPL_HWLOC_PREFIX
      CHPL_QTHREAD_CFG_OPTIONS += --with-hwloc-symbol-prefix=$(CHPL_HWLOC_PREFIX)hwloc_
    endif
    # don't bother checking if we can link against hwloc
    CHPL_QTHREAD_CFG_OPTIONS += --disable-hwloc-configure-checks
  endif
endif

ifneq (, $(CHPL_QTHREAD_TOPOLOGY))
TOPOLOGY = $(CHPL_QTHREAD_TOPOLOGY)
endif

ifneq (, $(TOPOLOGY))
CHPL_QTHREAD_CFG_OPTIONS += --with-topology=$(TOPOLOGY)
endif

# Used to create qthread-chapel.h
ifeq ($(TOPOLOGY),binders)
  USE_TOPOLOGY_BINDERS=1
else
  USE_TOPOLOGY_BINDERS=0
endif

# Have qthreads use Chapel's allocator, unless directed not to
ifeq (, $(call isTrue, $(CHPL_QTHREAD_NO_CHPL_ALLOC)))
  CHPL_QTHREAD_CFG_OPTIONS += --with-alloc=chapel
  CFLAGS_NEEDS_RT_INCLUDES = y
endif

FAILBUILD=
ifeq ($(CFLAGS_NEEDS_RT_INCLUDES), y)
  #
  # When building the Chapel allocator file we need all the Chapel runtime
  # defs and include directories, plus the third-party include dirs other
  # than that for Qthreads itself (since we're building it).  Notably, we
  # do not want -DNDEBUG; if we have that set when building Qthreads it
  # seems to cause Chapel programs built with the result to hang during
  # exit when shutting down Qthreads.
  #
  # Note that we call compileline with CHPL_MAKE_COMM=none. Under
  # CHPL_MAKE_COMM=gasnet, compileline will fail if gasnet is not built,
  # so in order to avoid ordering/dependencies just ignore the comm
  # setting since it does not impact the qthreads build. We don't want
  # to reset the CHPLENV_CACHE since the regular CHPL_COMM setting could
  # impact things like CHPL_TARGET_CPU, which is part of the build path
  #
  # Throw COMPILELINE_STDERR_REDIRECT= if compileline failure is reported
  # and you want to see the stderr from that.
  #
  COMPILELINE_STDERR_REDIRECT=2> /dev/null
  COMM_NONE_CHPLENV_CACHE := $(shell echo "$(CHPL_MAKE_CHPLENV_CACHE)" | sed 's/|CHPL_MAKE_COMM=[^|]*|/|CHPL_MAKE_COMM=none|/')
  INCS_DEFS := \
    $(shell CHPL_HOME=$(CHPL_MAKE_HOME) CHPL_MAKE_CHPLENV_CACHE="$(COMM_NONE_CHPLENV_CACHE)" $(CHPL_MAKE_HOME)/util/config/compileline --includes-and-defines \
               $(COMPILELINE_STDERR_REDIRECT) \
            || echo compilelineFAILURE)
  ifneq (, $(findstring compilelineFAILURE,$(INCS_DEFS)))
    FAILBUILD=y
  else
    INCS_DEFS_PROCESSED := \
      $(shell echo $(INCS_DEFS) \
              | tr ' ' '\n' \
              | grep '^-DCHPL\|/runtime//*include\|/third-party/.*/install' \
              | grep -v '/third-party/qthread/install' \
              | tr '\n' ' ' \
              | sed 's/ $$//')
    CFLAGS += $(INCS_DEFS_PROCESSED)
  endif
endif

# enable guard pages for stack overflow detection, unless directed not to
HAVE_GUARD_PAGES = 0
ifeq (, $(call isTrue, $(CHPL_QTHREAD_NO_GUARD_PAGES)))
HAVE_GUARD_PAGES = 1
CHPL_QTHREAD_CFG_OPTIONS += --enable-guard-pages
endif

# optionally enable printing compilation commands, etc. (to help diagnose
# build problems)
ifneq (, $(CHPL_QTHREAD_LOUD_RULES))
CHPL_QTHREAD_CFG_OPTIONS += --disable-silent-rules
endif

CHPL_QTHREAD_CFG_OPTIONS += --enable-static --disable-shared

# determine which scheduler to use based on locale model.  Override with a user
# provided option if they requested one.
SCHEDULER = nemesis
ifeq ($(CHPL_MAKE_LOCALE_MODEL),numa)
  # The distrib scheduler is numa aware (you can say things like "run this new
  # task on numa domain X" or "which numa domain is this task running on?");
  # however its performance is generally worse than nemesis so we don't use it
  # by default.
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

# Address sanitizer doesn't work with asm context switching so try
# makecontext/swapcontext (which may have false-positives, but doesn't
# immediately fail)
ifneq (,$(CHPL_QTHREAD_SANITIZER_SUPPORT))
  CHPL_QTHREAD_CFG_OPTIONS += --disable-fastcontext
endif

# No fastcontext on m1, fallback to ucontext (with _XOPEN_SOURCE to allow it)
ifeq ($(CHPL_MAKE_TARGET_PLATFORM),darwin)
  ifeq ($(CHPL_MAKE_TARGET_ARCH),arm64)
    CHPL_QTHREAD_CFG_OPTIONS += --disable-fastcontext
    CFLAGS += -D_XOPEN_SOURCE
  endif
endif

ifeq ($(CHPL_LIB_PIC),pic)
CFLAGS += $(SHARED_LIB_CFLAGS)
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
	@test -z "$(FAILBUILD)" || ( echo 'compileline failed!' && false )

#
# These first few lines touch a bunch of autoconf-oriented files in a
# certain order to prevent autoconf from running again; otherwise, we
# ran into issues if a user's autoconf environment was not as far
# ahead in version numbers as that which was used when packaging the
# Qthreads release
#
	cd $(QTHREAD_SUBDIR) && touch -c configure.ac
	cd $(QTHREAD_SUBDIR) && find . -name "*.m4" | xargs touch
	sleep 1
	cd $(QTHREAD_SUBDIR) && touch -c aclocal.m4
	sleep 1
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
ONE_WORKER_PER_SHEPHERD = 1
TASKS_CAN_MIGRATE_THREADS = 0
else ifeq ($(SCHEDULER),$(findstring $(SCHEDULER),distrib nottingham sherwood))
ONE_WORKER_PER_SHEPHERD = 0
TASKS_CAN_MIGRATE_THREADS = 1
else
$(error Unrecognized Qthreads scheduler '$(SCHEDULER)')
endif

qthread-chapel-h: FORCE
	echo "#define CHPL_QTHREAD_SCHEDULER_ONE_WORKER_PER_SHEPHERD" \
	     $(ONE_WORKER_PER_SHEPHERD) \
	     > $(QTHREAD_INSTALL_DIR)/include/qthread-chapel.h
	echo "#define CHPL_QTHREAD_TASKS_CAN_MIGRATE_THREADS" \
	     $(TASKS_CAN_MIGRATE_THREADS) \
	     >> $(QTHREAD_INSTALL_DIR)/include/qthread-chapel.h
	echo "#define CHPL_QTHREAD_HAVE_GUARD_PAGES" \
	     $(HAVE_GUARD_PAGES) \
	     >> $(QTHREAD_INSTALL_DIR)/include/qthread-chapel.h
	echo "#define CHPL_QTHREAD_TOPOLOGY_BINDERS" \
	     $(USE_TOPOLOGY_BINDERS) \
	     >> $(QTHREAD_INSTALL_DIR)/include/qthread-chapel.h

qthread: qthread-config qthread-build qthread-chapel-h

qthread-reconfig:
	cd $(QTHREAD_SUBDIR) && autoreconf -f -i

FORCE:

.NOTPARALLEL:
