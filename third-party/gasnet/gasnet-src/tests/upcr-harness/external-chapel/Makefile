CHAPEL_GIT_REPO   ?= chapel-lang/chapel
CHAPEL_GIT_COMMIT ?= master
CHAPEL_MAKE_ARGS  ?= -j4
CHAPEL_BLDDIR     ?= $(TOP_BUILDDIR)/chapel
CHAPEL_TMPDIR     ?= $(TOP_BUILDDIR)/chapel-tmp
CHAPEL_CONDUIT    ?= $(NETWORK) # Default to same conduit as the enclosing harness run
CHAPEL_PATH_PRE   ?=# empty - if set must end with :
CHAPEL_PATH_POST  ?=# empty - if set must start with :

HARNESS_LOGDIR    ?= .
LOGFILE           ?= $(HARNESS_LOGDIR)/chapel.log

# Paths and options for standard tools
PERL              ?= perl
WGET              ?= wget -nv
GZCAT             ?= gzip -cd
TAR               ?= tar
UNZIP             ?= unzip
P7ZIP             ?= 7za

# Must not inherit these from harness environment:
unexport LD LDFLAGS LIBS
unexport CC CFLAGS
unexport CXX CXXFLAGS
unexport AR RANLIB

# Parameterized support for download/unpack
CHAPEL_URL      ?= https://github.com/$(CHAPEL_GIT_REPO)/archive/$(CHAPEL_GIT_COMMIT)$(chapel_dl_suffix)
CHAPEL_ARCHIVE  ?= $(notdir $(CHAPEL_URL))
CHAPEL_DOWNLOAD ?= unzip # one of "unzip", "p7zip" or "tgz"
chapel-download: force
	rm -Rf $(CHAPEL_BLDDIR) $(CHAPEL_TMPDIR) && mkdir $(CHAPEL_TMPDIR)
	cd $(CHAPEL_TMPDIR) && $(WGET) $(CHAPEL_URL) 2>&1
	$(MAKE) -C $(CHAPEL_TMPDIR) -f `pwd`/Makefile chapel-unpack-$(CHAPEL_DOWNLOAD)
	rm $(CHAPEL_TMPDIR)/$(CHAPEL_ARCHIVE)
	mv $(CHAPEL_TMPDIR)/* $(CHAPEL_BLDDIR) # archive's root dir will vary
	rmdir $(CHAPEL_TMPDIR)
# Three ways to unpack the archive:
#  Option 1: "unzip" - .zip w/ unzip
#   This is the favored approach because it gives us the hash and uses a widely available utility.
chapel-unpack-unzip: force; $(UNZIP) -z $(CHAPEL_ARCHIVE) && $(UNZIP) -q $(CHAPEL_ARCHIVE)
chapel_dl_suffix_unzip = .zip
#  Option 2: "p7zip" - .zip w/ 7za (from p7zip package)
#   This also gives us the hash, but uses a less widely available utility.
#   However, it is sometimes necessary because many unzip installations contain a bug w.r.t. symlinks
chapel-unpack-p7zip: force; $(P7ZIP) x -bd $(CHAPEL_ARCHIVE)
chapel_dl_suffix_p7zip = .zip
#  Option 3: "tgz" - tar + gzip
#   This is the most portable, but it sacrifices having the git hash in the log file.
chapel-unpack-tgz:   force; $(GZCAT) $(CHAPEL_ARCHIVE) | $(TAR) xf -
chapel_dl_suffix_tgz   = .tar.gz
####
chapel_dl_suffix = $(chapel_dl_suffix_$(strip $(CHAPEL_DOWNLOAD)))
.PHONY: chapel-download chapel-unpack-unzip chapel-unpack-p7zip chapel-unpack-tgz

# Save time by building for a single conduit
CHPL_GASNET_CFG_OPTIONS += --disable-auto-conduit-detect --enable-$(CHAPEL_CONDUIT)

# Define CHPL_LAUNCHER for some conduits without defaults.
# Note that these have not necessarily all been tested.
chapel_launcher_opt = $(chapel_launcher_$(strip $(CHAPEL_CONDUIT)))
chapel_launcher_ofi = CHPL_LAUNCHER=gasnetrun_ofi
chapel_launcher_psm = CHPL_LAUNCHER=gasnetrun_psm
chapel_launcher_pami = CHPL_LAUNCHER=gasnetrun_pami

# Kludge some launchers that can be constructed as a simple delta from another:
chapel-launcher: force
	case $(CHAPEL_CONDUIT) in \
	ofi) $(MAKE) clone-launcher SRC_CONDUIT=ibv DST_CONDUIT=$(CHAPEL_CONDUIT) ;; \
	pami) $(MAKE) clone-launcher SRC_CONDUIT=mpi DST_CONDUIT=$(CHAPEL_CONDUIT) ;; \
	esac
clone-launcher: force
	cd $(CHAPEL_BLDDIR)/runtime/src/launch && \
	cp -Rp gasnetrun_$(SRC_CONDUIT) gasnetrun_$(DST_CONDUIT) && \
	cd gasnetrun_$(DST_CONDUIT) && \
	mv launch-gasnetrun_$(SRC_CONDUIT).c launch-gasnetrun_$(DST_CONDUIT).c && \
	$(PERL) -pi -e 's/_$(SRC_CONDUIT)/_$(DST_CONDUIT)/g' -- *
.PHONY: chapel-launcher clone-launcher


COMMON_ENV = CHPL_COMM=gasnet CHPL_COMM_SUBSTRATE=$(CHAPEL_CONDUIT) $(chapel_launcher_opt) \
	PATH=$(CHAPEL_PATH_PRE)$$PATH$(CHAPEL_PATH_POST)

# Build Chapel runtime
chapel: force
	rm -f chapel-built
	$(MAKE) chapel-download 2>&1 | tee $(LOGFILE)
	$(MAKE) chapel-launcher
	cd $(CHAPEL_BLDDIR)/third-party/gasnet && mv gasnet-src gasnet-src.dist
	cp -Rp $(TOP_SRCDIR)/gasnet $(CHAPEL_BLDDIR)/third-party/gasnet/gasnet-src
	cd $(CHAPEL_BLDDIR) && \
	source util/setchplenv.bash && \
	  env $(COMMON_ENV) $(CHAPEL_BUILD_ENV) \
	    $(MAKE) $(CHAPEL_MAKE_ARGS) all >> $(LOGFILE) 2>&1
	@echo '#!/bin/sh' > $@ ; chmod +x $@
	@touch chapel-built

# Not reliable other than on UDP, and wants to *run* the test
check: chapel-built
	source $(CHAPEL_BLDDIR)/util/setchplenv.bash && \
	  env $(COMMON_ENV) $(CHAPEL_CHECK_ENV) \
	    $(MAKE) -C $(CHAPEL_BLDDIR) check
	@echo '#!/bin/sh' > $@ ; chmod +x $@

#
# Build one test
#
test_common: chapel-built
	rm -f $(TEST_EXE){,_real}
	source $(CHAPEL_BLDDIR)/util/setchplenv.bash && \
	  env $(COMMON_ENV) $(CHAPEL_TEST_ENV) \
	    $(MAKE) -C $(CHAPEL_BLDDIR)/$(TEST_DIR) $(REAL_EXE) CHPL_FLAGS+="$(chpl_flags_$(TEST_EXE))" 2>&1
	mv $(CHAPEL_BLDDIR)/$(TEST_DIR)/$(REAL_EXE) $(TEST_EXE)
	if test -e $(CHAPEL_BLDDIR)/$(TEST_DIR)/$(REAL_EXE)_real; then \
	  mv $(CHAPEL_BLDDIR)/$(TEST_DIR)/$(REAL_EXE)_real $(TEST_EXE)_real; \
	fi
one_test: force  # Using the tests natural name
	$(MAKE) test_common REAL_EXE=$(TEST_EXE)
alt_test: force  # Using an alternate test name
	$(MAKE) test_common
.PHONY: test_common one_test alt_test


# top-level examples
EXAMPLES = hello hello2-module hello3-datapar hello4-datapar-dist hello5-taskpar hello6-taskpar-dist
$(EXAMPLES): force; $(MAKE) one_test TEST_EXE=$@ TEST_DIR=examples

# directory examples/benchmarks
BENCHMARKS = lcals lulesh miniMD
$(BENCHMARKS): force; $(MAKE) one_test TEST_EXE=$@ TEST_DIR=examples/benchmarks/$@

# directory examples/benchmarks/hpcc
HPCC = stream stream-ep ra ra-atomics fft ptrans hpl
$(HPCC): force; $(MAKE) one_test TEST_EXE=$@ TEST_DIR=examples/benchmarks/hpcc
chpl_flags_fft = --ieee-float
chpl_flags_ra = -O -s useOn=true
chpl_flags_ra-atomics = -O
unexport $(addprefix chpl_flags_,$(HPCC))

# directory examples/benchmarks/isx
ISX = isx-strong isx-weak isx-weakISO
$(ISX): force; $(MAKE) alt_test TEST_EXE=$@ REAL_EXE=isx TEST_DIR=examples/benchmarks/isx

# directory examples/benchmarks/ssca2
SSCA2 = ssca2_rmat ssca2_1d ssca2_2d ssca2_3d ssca2_4d
$(SSCA2): force; $(MAKE) alt_test TEST_EXE=$@ REAL_EXE=SSCA2_main TEST_DIR=examples/benchmarks/ssca2
chpl_flags_ssca2_common = -M SSCA2_Modules SSCA2_Modules/SSCA2_kernels.chpl SSCA2_Modules/analyze_torus_graphs_stencil_rep_v1.chpl -sREPRODUCIBLE_PROBLEMS=true -sPRINT_TIMING_STATISTICS=true -sVALIDATE_BC=true
chpl_flags_ssca2_rmat = $(chpl_flags_ssca2_common) -sDEBUG_NEIGHBOR_HISTOGRAM -sBUILD_RMAT_VERSION=true -sBUILD_1D_TORUS_VERSION=false -sBUILD_2D_TORUS_VERSION=false -sBUILD_3D_TORUS_VERSION=false -sBUILD_4D_TORUS_VERSION=false
chpl_flags_ssca2_1d   = $(chpl_flags_ssca2_common) -sBUILD_RMAT_VERSION=false -sBUILD_1D_TORUS_VERSION=true -sBUILD_2D_TORUS_VERSION=false -sBUILD_3D_TORUS_VERSION=false -sBUILD_4D_TORUS_VERSION=false
chpl_flags_ssca2_2d   = $(chpl_flags_ssca2_common) -sBUILD_RMAT_VERSION=false -sBUILD_1D_TORUS_VERSION=false -sBUILD_2D_TORUS_VERSION=true -sBUILD_3D_TORUS_VERSION=false -sBUILD_4D_TORUS_VERSION=false
chpl_flags_ssca2_3d   = $(chpl_flags_ssca2_common) -sBUILD_RMAT_VERSION=false -sBUILD_1D_TORUS_VERSION=false -sBUILD_2D_TORUS_VERSION=false -sBUILD_3D_TORUS_VERSION=true -sBUILD_4D_TORUS_VERSION=false
chpl_flags_ssca2_4d   = $(chpl_flags_ssca2_common) -sBUILD_RMAT_VERSION=false -sBUILD_1D_TORUS_VERSION=false -sBUILD_2D_TORUS_VERSION=false -sBUILD_3D_TORUS_VERSION=false -sBUILD_4D_TORUS_VERSION=true
unexport $(addprefix chpl_flags_,$(SSCA2) ssca2_common)

# directory examples/primers
PRIMERS = \
        arrays arrayVectorOps associative atomics chpldoc.doc classes         \
        distributions domains fileIO genericClasses iterators locales         \
        modules opaque parIters procedures randomNumbers ranges reductions    \
        slices sparse syncsingle taskParallel timers varargs variables        \
        voidVariables
$(PRIMERS): force; $(MAKE) one_test TEST_EXE=$@ TEST_DIR=examples/primers/$@

# directory examples/programs
PROGRAMS = beer genericStack jacobi linkedList norm prodCons quicksort tree
$(PROGRAMS): force; $(MAKE) one_test TEST_EXE=$@ TEST_DIR=examples/programs/$@

force:

.PHONY: force
