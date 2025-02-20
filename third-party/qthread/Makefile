ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

CMAKE ?= cmake

export CHPL_MAKE_LOCALE_MODEL

#
# set up the directories
#
QTHREAD_ABS_DIR = $(shell pwd)
QTHREAD_INSTALL_DIR = $(QTHREAD_ABS_DIR)/$(QTHREAD_INSTALL_SUBDIR)
QTHREAD_BUILD_DIR = $(QTHREAD_ABS_DIR)/$(QTHREAD_BUILD_SUBDIR)
QTHREAD_DIR = $(QTHREAD_ABS_DIR)

ifneq ($(CHPL_MAKE_HWLOC),none)
  # Have Qthreads get its hwloc topology from the Chapel runtime,
  # unless directed not to.
  TOPOLOGY=binders
  ifeq (, $(call isTrue, $(CHPL_QTHREAD_DONT_GET_TOPO_FROM_RT)))
    CFLAGS_NEEDS_RT_INCLUDES = y
    CHPL_QTHREAD_CFG_OPTIONS += -DQTHREADS_HWLOC_GET_TOPOLOGY_FUNCTION="chpl_topo_getHwlocTopology()"
  endif
  ifeq ($(CHPL_MAKE_HWLOC),bundled)
    CHPL_QTHREAD_CFG_OPTIONS += -DHWLOC_INSTALL_DIR=$(HWLOC_INSTALL_DIR)
  else ifeq ($(CHPL_MAKE_HWLOC),system)
    CHPL_QTHREAD_CFG_OPTIONS += -DHWLOC_INSTALL_DIR=$(shell $(CHPL_MAKE_PYTHON) $(CHPL_MAKE_HOME)/util/chplenv/chpl_hwloc.py --prefix)
  endif
endif

ifneq (, $(CHPL_QTHREAD_TOPOLOGY))
TOPOLOGY = $(CHPL_QTHREAD_TOPOLOGY)
endif

ifneq (, $(TOPOLOGY))
CHPL_QTHREAD_CFG_OPTIONS += -DQTHREADS_TOPOLOGY=$(TOPOLOGY)
endif

# Used to create qthread-chapel.h
ifeq ($(TOPOLOGY),binders)
  USE_TOPOLOGY_BINDERS=1
else
  USE_TOPOLOGY_BINDERS=0
endif

# Have qthreads use Chapel's allocator, unless directed not to
ifeq (, $(call isTrue, $(CHPL_QTHREAD_NO_CHPL_ALLOC)))
  CHPL_QTHREAD_CFG_OPTIONS += -DQTHREADS_ALLOC=chapel
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
	# We also need to provide paths to system hwloc and jemalloc if they are
  # enabled, since it might not be in default compiler search paths.
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
 	ifeq ($(CHPL_MAKE_HWLOC),system)
		SYSTEM_INCS_DEFS := $(SYSTEM_INCS_DEFS)\|hwloc
  endif
	ifeq ($(CHPL_MAKE_TARGET_JEMALLOC),system)
	SYSTEM_INCS_DEFS := $(SYSTEM_INCS_DEFS)\|jemalloc
  endif

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
              | grep '^-DCHPL\|/runtime//*include\|/third-party/.*/install$(SYSTEM_INCS_DEFS)' \
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
CHPL_QTHREAD_CFG_OPTIONS += -DQTHREADS_GUARD_PAGES=ON
endif

ifneq (, $(CHPL_QTHREAD_LOUD_RULES))
CHPL_QTHREAD_CFG_OPTIONS += -DCMAKE_VERBOSE_MAKEFILE=ON
endif

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
CHPL_QTHREAD_CFG_OPTIONS += -DQTHREADS_SCHEDULER=$(SCHEDULER)

# only do this on M1+ Macs
ifeq ($(CHPL_MAKE_TARGET_ARCH),arm64)
ifeq ($(CHPL_MAKE_TARGET_PLATFORM),darwin)
  CHPL_QTHREAD_CFG_OPTIONS += -DQTHREADS_CACHELINE_SIZE_ESTIMATE=128
endif
endif

# reduce performance penalty in cases where numChapelTasks < numQthreadWorkers
CHPL_QTHREAD_CFG_OPTIONS += -DQTHREADS_CONDWAIT_QUEUE=ON

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
	mkdir -p $(QTHREAD_BUILD_DIR)
	$(CMAKE) -B $(QTHREAD_BUILD_DIR) -S $(QTHREAD_SUBDIR) \
	$(CHPL_QTHREAD_CFG_OPTIONS) \
	-DQTHREADS_BUILD_TESTS=OFF \
	-DCMAKE_INSTALL_PREFIX='$(QTHREAD_INSTALL_DIR)' \
	-DCMAKE_INSTALL_LIBDIR='$(QTHREAD_LIB_DIR)' \
	-DBUILD_SHARED_LIBS=OFF \
	-DCMAKE_C_COMPILER='$(CC)' \
	-DCMAKE_C_FLAGS='$(CFLAGS)' \
	-DCMAKE_CXX_COMPILER='$(CXX)' \
	-DCMAKE_CXX_FLAGS='$(CXXFLAGS)' \
	-DCMAKE_MAKE_PROGRAM='$(MAKE)'

qthread-build: FORCE
	if [ -f $(QTHREAD_BUILD_DIR)/Makefile ]; then cd $(QTHREAD_BUILD_DIR) && $(MAKE); else $(CMAKE) --build $(QTHREAD_BUILD_DIR); fi

qthread-install: FORCE
	if [ -f $(QTHREAD_BUILD_DIR)/Makefile ]; then cd $(QTHREAD_BUILD_DIR) && $(MAKE) install; else $(CMAKE) --install $(QTHREAD_BUILD_DIR); fi

FORCE:

.NOTPARALLEL:

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

qthread: qthread-config qthread-build qthread-install qthread-chapel-h

FORCE:

.NOTPARALLEL:
