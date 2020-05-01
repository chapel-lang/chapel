LEGION_GIT_REPO   ?= StanfordLegion/legion
LEGION_GIT_COMMIT ?= stable
LEGION_BLDDIR     ?= $(TOP_BUILDDIR)/legion
LEGION_TMPDIR     ?= $(TOP_BUILDDIR)/legion-tmp
LEGION_GASNET     ?= # Path to an existing installation, if any
LEGION_CONDUIT    ?= $(NETWORK) # Default to same conduit as the enclosing harness run
LEGION_MAKE_ARGS  ?= -j4

HARNESS_LOGDIR    ?= .
LOGFILE = $(HARNESS_LOGDIR)/legion.log
TO_LOG = >> $(LOGFILE) 2>&1

# Paths and options for standard tools
WGET              ?= wget -nv
GZCAT             ?= gzip -cd
TAR               ?= tar
UNZIP             ?= unzip
P7ZIP             ?= 7za
GIT               ?= git

# Strip whitespace to simplify later use
LEGION_GASNET  := $(strip $(LEGION_GASNET))
LEGION_CONDUIT := $(strip $(LEGION_CONDUIT))

# Logic to extract GASNet's compiler variables.
# We use a sub-shell to avoid pulling in the entire makefile fragment.
# TODO: may want to handle LDFLAGS here as well?
GASNET_VARS = $(shell $(MAKE) --no-print-directory echovars FRAGMENT=$(LEGION_CONDUIT))
FRAGMENT=#empty by default
ifneq ($(FRAGMENT),)
include $(LEGION_GASNET_INST)/include/$(FRAGMENT)-conduit/$(FRAGMENT)-par.mak
echovars: force; @echo "CC='$(GASNET_CC)' CXX='$(GASNET_CXX)'"
.PHONY: echovars
endif

RUNTIME_TARGETS = librealm.a liblegion.a
SEED_DIR = $(LEGION_BLDDIR)/examples/circuit # NOTE: must not be an OMP or CUDA test
COMMON_ENV = \
	LG_RT_DIR="$(LEGION_BLDDIR)/runtime" USE_GASNET=1 \
	GASNET="$(LEGION_GASNET_INST)" CONDUIT=$(LEGION_CONDUIT) \
	$(GASNET_VARS)
DO_MAKE = env $(COMMON_ENV) $(LEGION_TEST_ENV) $(MAKE) $(LEGION_MAKE_ARGS)

# Parametrized support for download/unpack
LEGION_DOWNLOAD ?=# one of "unzip", "p7zip" or "tgz", default auto-detects using LEGION_URL suffix
LEGION_DOWNLOAD := $(strip $(LEGION_DOWNLOAD))
legion_dl_suffix_unzip := .zip
legion_dl_suffix_p7zip := .zip
legion_dl_suffix_tgz   := .tar.gz
legion_dl_suffix=$(legion_dl_suffix_$(LEGION_DOWNLOAD))
ifeq ($(strip $(legion_dl_suffix)),)
legion_dl_suffix := .zip
endif

LEGION_URL      ?= https://github.com/$(LEGION_GIT_REPO)/archive/$(LEGION_GIT_COMMIT)$(legion_dl_suffix)
LEGION_ARCHIVE  ?= $(notdir $(LEGION_URL))
legion-download: force
	rm -Rf $(LEGION_BLDDIR) $(LEGION_TMPDIR) && mkdir $(LEGION_TMPDIR)
	cd $(LEGION_TMPDIR) && $(WGET) $(LEGION_URL) 2>&1 || ( cat wget-log 2> /dev/null ; exit 3 ) || exit $$?
	cd $(LEGION_TMPDIR) && cat wget-log 2> /dev/null ; rm -f wget-log ; exit 0
	@LEGION_TESTDIR=`pwd` && \
	 cd $(LEGION_TMPDIR) && LEGION_ARCHIVE=`/bin/ls` && LEGION_DOWNLOAD="$(LEGION_DOWNLOAD)" && \
	 if test -z "$$LEGION_DOWNLOAD" ; then \
	   case "$$LEGION_ARCHIVE" in \
	     *.zip)          LEGION_DOWNLOAD=unzip ;; \
	     *.p7z|*.7z)     LEGION_DOWNLOAD=p7zip ;; \
	     *.tar.gz|*.tgz) LEGION_DOWNLOAD=tgz   ;; \
	     *) echo "ERROR: Unknown archive suffix on '$$LEGION_ARCHIVE': Set LEGION_DOWNLOAD=(unzip,p7zip,tgz)" ; exit 1; \
	   esac \
	 fi && \
	 echo Fetched $$LEGION_ARCHIVE : LEGION_DOWNLOAD=$$LEGION_DOWNLOAD && \
	 $(MAKE) -f $$LEGION_TESTDIR/Makefile legion-unpack-$$LEGION_DOWNLOAD LEGION_ARCHIVE="$$LEGION_ARCHIVE" && \
	 rm -f $(LEGION_TMPDIR)/$$LEGION_ARCHIVE
	mv $(LEGION_TMPDIR)/* $(LEGION_BLDDIR) # archive's root dir will vary
	rmdir $(LEGION_TMPDIR)
# Three ways to unpack the archive:
#  Option 1: "unzip" - .zip w/ unzip
#   This is the favored approach because it gives us the hash and uses a widely available utility.
legion-unpack-unzip: force; $(UNZIP) -z $(LEGION_ARCHIVE) && $(UNZIP) -q $(LEGION_ARCHIVE)
#  Option 2: "p7zip" - .zip w/ 7za (from p7zip package)
#   This also gives us the hash, but uses a less widely available utility.
#   However, it is sometimes necessary because many unzip installations contain a bug w.r.t. symlinks
legion-unpack-p7zip: force; $(P7ZIP) x -bd $(LEGION_ARCHIVE)
#  Option 3: "tgz" - tar + gzip
#   This is the most portable, but cannot extract the hash unless git is available
legion-unpack-tgz:   force
	-$(GZCAT) $(LEGION_ARCHIVE) | $(GIT) get-tar-commit-id; exit 0
	$(GZCAT) $(LEGION_ARCHIVE) | $(TAR) xf -
####
.PHONY: legion-download legion-unpack-unzip legion-unpack-p7zip legion-unpack-tgz

# Apply upsteam or local patches, if any
legion_patches =
legion-patch: force
	cd $(LEGION_BLDDIR) && \
	for p in $(legion_patches); do $(WGET) $$p && patch -p1 < `basename $$p`; done
.PHONY: legion-patch

# Either install GASNet (empty LEGION_GASNET) or use in-place
ifeq ($(LEGION_GASNET),)
export LEGION_GASNET_INST = $(LEGION_BLDDIR)/gasnet
legion-gasnet: force
	if test -d "$(LEGION_GASNET_INST)"; then \
	  rm -Rf "$(LEGION_GASNET_INST)";  \
	elif test -e "$(LEGION_GASNET_INST)"; then \
	  rm -f "$(LEGION_GASNET_INST)";  \
	fi
	@echo ======== Installing GASNet at $(LEGION_GASNET_INST) ========
	$(MAKE) install -C $(TOP_BUILDDIR)/gasnet prefix="$(LEGION_GASNET_INST)"
else
export LEGION_GASNET_INST = $(LEGION_GASNET)
legion-gasnet: force
	@if test -f "$(LEGION_GASNET_INST)/include/gasnet.h"; then \
	  echo ======== Using GASNet pre-installed at $(LEGION_GASNET_INST) ========; \
	else \
	  echo ERROR: No GASNet install at $(LEGION_GASNET_INST) \
	  exit 1; \
	fi
endif
.PHONY: legion-gasnet

# Symlink or simple script that corresponds to
#    RunCmd = ./launcher -np %N %P %A
launcher:
	case $(LEGION_CONDUIT) in \
	smp) echo '#!/bin/sh' > $@ ; \
	     echo 'N=$$2;shift 2;eval env GASNET_PSHM_NODES=$$N "$$@"' >> $@ ; \
	     chmod +x $@;; \
	udp) ln -s "$(LEGION_GASNET_INST)"/bin/amudprun $@;; \
	  *) ln -s "$(LEGION_GASNET_INST)"/bin/gasnetrun_$(LEGION_CONDUIT) $@;; \
	esac

# Pre-build the runtime libs to separate the output from that of first test build
rtlibs: force
	( echo ======== Prebuild Legion RT libs in $(SEED_DIR) ========; \
	  $(DO_MAKE) -C $(SEED_DIR) $(RUNTIME_TARGETS) \
	) $(TO_LOG)
.PHONY: rtlibs

_legion_clean: force
	( echo ======== Cleaning Legion RT libs via $(SEED_DIR) ========; \
	  $(DO_MAKE) -C $(SEED_DIR) clean \
	) $(TO_LOG)
	@echo '#!/bin/sh' > $@ ; chmod +x $@
.PHONY: _legion_clean
	
_legion: force
	rm -Rf legion-built launcher $(LOGFILE)
	$(call safe_tee_dance, $(MAKE) legion-download)
	$(call safe_tee_dance, $(MAKE) legion-patch)
	$(PERL) -pi -e s,mpi_interop,realm_saxpy,g -- $(LEGION_BLDDIR)/test.py # replace bogus test
	$(call safe_tee_dance, $(MAKE) legion-gasnet)
	@echo '#!/bin/sh' > $@ ; chmod +x $@
	$(MAKE) launcher rtlibs
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
	( cd $(LEGION_BLDDIR)/$(TEST_DIR) &&               \
          rm -f $(TEST_EXE).o $(RUNTIME_TARGETS) || exit 0 \
	) $(TO_LOG)

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

# Run a command, with output to both stdout and $(LOGFILE), preserving exit code of the command
# Based on https://unix.stackexchange.com/questions/14270/get-exit-status-of-process-thats-piped-to-another/70675#70675
safe_tee_dance = (((($1; echo $$? >&3) 2>&1 | tee -a $(LOGFILE) >&4) 3>&1) | (read rc; exit $$rc)) 4>&1
