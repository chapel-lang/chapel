ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/..
endif
include $(CHPL_MAKE_HOME)/make/Makefile.base
include $(THIRD_PARTY_DIR)/chpl-venv/Makefile.include

ifdef CHPL_DEVELOPER
DEBUG=1
export DEBUG
WARNINGS=1
export WARNINGS
else
OPTIMIZE=1
export OPTIMIZE
endif

default: all

all: test-venv chpldoc-venv amudprun gasnet gmp hwloc jemalloc libunwind qthread llvm

clean: FORCE
	cd chpl-venv && $(MAKE) clean
	cd amudprun && $(MAKE) clean
	cd gasnet && $(MAKE) clean
	cd gmp && $(MAKE) clean
	cd hwloc && $(MAKE) clean
	cd jemalloc && $(MAKE) clean
	cd libunwind && $(MAKE) clean
	cd llvm && $(MAKE) clean
	cd massivethreads && $(MAKE) clean
	cd qthread && $(MAKE) clean
	cd re2 && $(MAKE) clean
	cd fltk && $(MAKE) clean

cleanall: FORCE
	cd chpl-venv && $(MAKE) cleanall
	cd amudprun && $(MAKE) cleanall
	cd gasnet && $(MAKE) cleanall
	cd gmp && $(MAKE) cleanall
	cd hwloc && $(MAKE) cleanall
	cd jemalloc && $(MAKE) cleanall
	cd libunwind && $(MAKE) cleanall
	cd llvm && $(MAKE) cleanall
	cd massivethreads && $(MAKE) cleanall
	cd qthread && $(MAKE) cleanall
	cd re2 && $(MAKE) cleanall
	cd fltk && $(MAKE) cleanall

clobber: FORCE
	cd chpl-venv && $(MAKE) clobber
	cd amudprun && $(MAKE) clobber
	cd gasnet && $(MAKE) clobber
	cd gmp && $(MAKE) clobber
	cd hwloc && $(MAKE) clobber
	cd jemalloc && $(MAKE) clobber
	cd libunwind && $(MAKE) clobber
	cd llvm && $(MAKE) clobber
	cd massivethreads && $(MAKE) clobber
	cd qthread && $(MAKE) clobber
	cd re2 && $(MAKE) clobber
	cd fltk && $(MAKE) clobber

depend:

test-venv: $(CHPL_VENV_TEST_REQS)
$(CHPL_VENV_TEST_REQS): $(CHPL_VENV_TEST_REQUIREMENTS_FILE)
	cd chpl-venv && $(MAKE) test-venv

chpldoc-venv: $(CHPL_VENV_SPHINX_BUILD)
$(CHPL_VENV_SPHINX_BUILD): $(CHPL_VENV_CHPLDOC_REQUIREMENTS_FILE)
	cd chpl-venv && $(MAKE) chpldoc-venv

amudprun: $(AMUDPRUN_INSTALL_DIR)
$(AMUDPRUN_INSTALL_DIR):
	cd amudprun && $(MAKE)

# See gasnet/Makefile for explanation of the post-install step
gasnet: $(GASNET_INSTALL_DIR)
$(GASNET_INSTALL_DIR):
	cd gasnet && $(MAKE) && $(MAKE) post-install

try-gmp: FORCE
ifeq ($(wildcard $(GMP_BUILD_DIR)),)
	@echo "Speculatively attempting to build gmp"
	-@$(MAKE) GMP_SPECULATIVE=yes gmp
else ifeq ($(wildcard $(GMP_H_FILE)),)
	$(info Speculative build of gmp squashed due to previous failures.)
endif

gmp: $(GMP_H_FILE)
$(GMP_H_FILE):
	cd gmp && $(MAKE)

hwloc: $(HWLOC_INSTALL_DIR)
$(HWLOC_INSTALL_DIR):
	cd hwloc && $(MAKE)

jemalloc: $(JEMALLOC_INSTALL_DIR)
$(JEMALLOC_INSTALL_DIR):
	cd jemalloc && $(MAKE)

libunwind: $(LIBUNWIND_INSTALL_DIR)
$(LIBUNWIND_INSTALL_DIR):
	cd libunwind && $(MAKE)

massivethreads: $(MASSIVETHREADS_INSTALL_DIR)
$(MASSIVETHREADS_INSTALL_DIR):
	cd massivethreads && $(MAKE)

ifeq ($(CHPL_MAKE_HWLOC), hwloc)
qthread: $(HWLOC_INSTALL_DIR) $(QTHREAD_INSTALL_DIR)
else
qthread: $(QTHREAD_INSTALL_DIR)
endif
$(QTHREAD_INSTALL_DIR):
	cd qthread && $(MAKE)
$(QTHREAD_ALIASES): qthread

llvm: FORCE
	cd llvm && $(MAKE)

libhdfs3: FORCE
	cd libhdfs3 && $(MAKE)

try-re2: FORCE
ifeq ($(wildcard $(RE2_BUILD_DIR)),)
	@echo "Speculatively attempting to build re2"
	-@$(MAKE) re2
else
ifeq ($(wildcard $(RE2_H_FILE)),)
	$(info Speculative build of re2 squashed due to previous failures.)
endif
endif

re2: $(RE2_H_FILE)
$(RE2_H_FILE):
	cd re2 && $(MAKE)

-include Makefile.devel

FORCE:

