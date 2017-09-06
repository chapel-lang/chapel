LEGION_GIT_REPO   ?= StanfordLegion/legion
LEGION_GIT_COMMIT ?= stable
LEGION_BLDDIR     ?= $(TOP_BUILDDIR)/legion
LEGION_TMPDIR     ?= $(TOP_BUILDDIR)/legion-tmp
LEGION_GASNET     ?= # Path to an existing installation, if any
LEGION_CONDUIT    ?= $(NETWORK) # Default to same conduit as the enclosing harness run

HARNESS_LOGDIR    ?= .
LOGFILE = $(HARNESS_LOGDIR)/legion.log
TO_LOG = >> $(LOGFILE) 2>&1

# Paths and options for standard tools
WGET              ?= wget -nv
GZCAT             ?= gzip -cd
TAR               ?= tar
UNZIP             ?= unzip
P7ZIP             ?= 7za

# Logic to extract GASNet's compiler variables.
# We use a sub-shell to avoid pulling in the entire makefile fragment.
# TODO: may want to handle LDFLAGS here as well?
GASNET_VARS = $(shell $(MAKE) --no-print-directory echovars FRAGMENT=$(strip $(LEGION_CONDUIT)))
FRAGMENT=#empty by default
ifneq ($(FRAGMENT),)
include $(LEGION_BLDDIR)/gasnet/include/$(FRAGMENT)-conduit/$(FRAGMENT)-par.mak
echovars: force; @echo "CC='$(GASNET_CC)' CXX='$(GASNET_CXX)'"
.PHONY: echovars
endif

RUNTIME_TARGETS = librealm.a liblegion.a
SEED_DIR = $(LEGION_BLDDIR)/examples/circuit # NOTE: must not be an OMP or CUDA test
COMMON_ENV = \
	LG_RT_DIR="$(LEGION_BLDDIR)/runtime" USE_GASNET=1 \
	GASNET="$(LEGION_BLDDIR)/gasnet" CONDUIT=$(LEGION_CONDUIT) \
	$(GASNET_VARS)
DO_MAKE = env $(COMMON_ENV) $(LEGION_TEST_ENV) $(MAKE)

# Parametrized support for download/unpack
LEGION_URL      ?= https://github.com/$(LEGION_GIT_REPO)/archive/$(LEGION_GIT_COMMIT)$(legion_dl_suffix)
LEGION_ARCHIVE  ?= $(notdir $(LEGION_URL))
LEGION_DOWNLOAD ?= unzip # one of "unzip", "p7zip" or "tgz"
legion-download: force
	rm -Rf $(LEGION_BLDDIR) $(LEGION_TMPDIR) && mkdir $(LEGION_TMPDIR)
	cd $(LEGION_TMPDIR) && $(WGET) $(LEGION_URL) 2>&1
	$(MAKE) -C $(LEGION_TMPDIR) -f `pwd`/Makefile legion-unpack-$(LEGION_DOWNLOAD)
	rm $(LEGION_TMPDIR)/$(LEGION_ARCHIVE)
	mv $(LEGION_TMPDIR)/* $(LEGION_BLDDIR) # archive's root dir will vary
	rmdir $(LEGION_TMPDIR)
# Three ways to unpack the archive:
#  Option 1: "unzip" - .zip w/ unzip
#   This is the favored approach because it gives us the hash and uses a widely available utility.
legion-unpack-unzip: force; $(UNZIP) -z $(LEGION_ARCHIVE) && $(UNZIP) -q $(LEGION_ARCHIVE)
legion_dl_suffix_unzip = .zip
#  Option 2: "p7zip" - .zip w/ 7za (from p7zip package)
#   This also gives us the hash, but uses a less widely available utility.
#   However, it is sometimes necessary because many unzip installations contain a bug w.r.t. symlinks
legion-unpack-p7zip: force; $(P7ZIP) x -bd $(LEGION_ARCHIVE)
legion_dl_suffix_p7zip = .zip
#  Option 3: "tgz" - tar + gzip
#   This is the most portable, but it sacrifices having the git hash in the log file.
legion-unpack-tgz:   force; $(GZCAT) $(LEGION_ARCHIVE) | $(TAR) xf -
legion_dl_suffix_tgz   = .tar.gz
####
legion_dl_suffix = $(legion_dl_suffix_$(strip $(LEGION_DOWNLOAD)))
.PHONY: legion-download legion-unpack-unzip legion-unpack-p7zip legion-unpack-tgz

# Symlink or simple script that corresponds to
#    RunCmd = ./launcher -np %N %P %A
launcher:
	case $(LEGION_CONDUIT) in \
	smp) echo '#!/bin/sh' > $@ ; \
	     echo 'N=$$2;shift 2;eval env GASNET_PSHM_NODES=$$N "$$@"' >> $@ ; \
	     chmod +x $@;; \
	udp) ln -s "$(LEGION_BLDDIR)"/gasnet/bin/amudprun $@;; \
	  *) ln -s "$(LEGION_BLDDIR)"/gasnet/bin/gasnetrun_$(LEGION_CONDUIT) $@;; \
	esac
	
legion: force
	rm -Rf legion-built launcher $(LOGFILE)
	$(MAKE) legion-download 2>&1 | tee $(LOGFILE)
	$(PERL) -pi -e s,mpi_interop,realm_saxpy,g -- $(LEGION_BLDDIR)/test.py # replace bogus test
	if test -f "$(LEGION_GASNET)/include/gasnet.h"; then \
	  echo ======== GASNet symlinked ========; \
	  ln -s `cd "$(LEGION_GASNET)" && pwd` $(LEGION_BLDDIR)/gasnet; \
	else \
	  echo ======== GASNet install ========; \
	  $(MAKE) install -C $(TOP_BUILDDIR)/gasnet prefix="$(LEGION_BLDDIR)/gasnet"; \
	fi $(TO_LOG)
	( echo ======== Prebuild Legion RT libs in $(SEED_DIR) ========; \
	  $(DO_MAKE) -C $(SEED_DIR) $(RUNTIME_TARGETS) \
	) $(TO_LOG)
	@echo '#!/bin/sh' > $@ ; chmod +x $@
	$(MAKE) launcher
	@touch legion-built

# Runs of test.py:
TESTS_PY = legion_cxx regent fuzzer realm external private perf
$(TESTS_PY): legion-built
	cd $(LEGION_BLDDIR) && \
	env $(COMMON_ENV) LAUNCHER="echo Would run: " $(LEGION_TEST_ENV) \
	    ./test.py --use gasnet --test $@ 2>&1
	@echo '#!/bin/sh' > $@ ; chmod +x $@


# Build a single test
one_test: legion-built
	( echo ======== Rebuild Legion RT libs in $(TEST_DIR) ========; \
	  $(DO_MAKE) -C $(LEGION_BLDDIR)/$(TEST_DIR) $(RUNTIME_TARGETS) \
	) $(TO_LOG)
	$(DO_MAKE) -C $(LEGION_BLDDIR)/$(TEST_DIR)
	mv $(LEGION_BLDDIR)/$(TEST_DIR)/$(TEST_EXE) .

# examples/:
EXAMPLES = circuit dynamic_registration ghost ghost_pull realm_saxpy spmd_cgsolver virtual_map 
$(EXAMPLES): force; $(MAKE) one_test TEST_DIR=examples/$@ TEST_EXE=$@

# tutorial/:
TUTORIALS = hello_world tasks_and_futures index_tasks global_vars logical_regions \
            physical_regions privileges partitioning multiple_partitions custom_mapper 
$(TUTORIALS): force; $(MAKE) one_test TEST_DIR=tutorial/??_$@ TEST_EXE=$@

# test/:
TESTS = attach_file_mini
$(TESTS): force; $(MAKE) one_test TEST_DIR=test/$@ TEST_EXE=$@

force:

.PHONY: force one_test $(TESTS_PY)
