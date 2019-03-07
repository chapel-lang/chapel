UPCXX_GIT_REPO   ?= berkeleylab/upcxx
UPCXX_GIT_COMMIT ?= develop
UPCXX_BLDDIR     ?= $(HARNESS_WORKDIR)/upcxx
UPCXX_BACKEND    ?= gasnet_seq
UPCXX_INSTDIR    ?= $(HARNESS_WORKDIR)/upcxx-inst
UPCXX_TMPDIR     ?= $(HARNESS_WORKDIR)/upcxx-tmp
UPCXX_CONDUIT    ?= $(NETWORK) # Default to same conduit as the enclosing harness run
UPCXX_EXTRA_FLAGS ?=# extra flags to add to both compilers, may be set by pushbuild/end-user
UPCXX_TEST_FLAGS  ?=# per-test flags reserved for use by harness.conf
export UPCXX_BACKEND

# Options for providing GASNET to nobs:
# 1. set UPCXX_GASNET to an existing build or install path to use
# The remaining options assume $(TOP_BUILDDIR)/gasnet is a valid build dir
# 2. set UPCXX_INSTALL_GASNET=1 to perform a GASNet install and use it
#    This one only uses the GASNet build dir during compile but not run.
# 3. set UPCXX_TARBALL_GASNET=1 to create a GASNet tarball and use it
#    This one notably implies nobs will configure and build GASNet,
#    although still uses the GASNet build dir during compile and run. 
# 4. Default is to point at the existing $(TOP_BUILDDIR)/gasnet build,
#    which must remain valid during both compile and run
ifneq ($(strip $(UPCXX_GASNET)),)
  UPCXX_EXTERNAL_GASNET:=1
  NOBS_GASNET:=$(UPCXX_GASNET)
else ifneq ($(strip $(UPCXX_INSTALL_GASNET)),)
  NOBS_GASNET:=$(UPCXX_BLDDIR)/gasnet
  UPCXX_LAUNCHERS:=$(UPCXX_BLDDIR)/gasnet
  UPCXX_FRAGMENTS:=$(TOP_BUILDDIR)/gasnet
else ifneq ($(strip $(UPCXX_TARBALL_GASNET)),)
  NOBS_GASNET:=$(UPCXX_BLDDIR)/gasnet.tar.gz
  UPCXX_FRAGMENTS:=$(TOP_BUILDDIR)/gasnet
  # UPCXX_LAUNCHERS default to TOP_BUILDDIR to avoid finding the nobs gasnet builddir
  # Don't pollute nobs configure with harness flags
  UNSET_VARS = CFLAGS CXXFLAGS CPPFLAGS LDFLAGS LIBS
  # use meta to prevent a spurious second configure
  UPCXX_USE_META = 1
else # default to using builddir
  NOBS_GASNET:=$(TOP_BUILDDIR)/gasnet
endif
UPCXX_FRAGMENTS ?= $(NOBS_GASNET)
UPCXX_LAUNCHERS ?= $(UPCXX_FRAGMENTS)
UPCXX_USE_META  ?= 1

HARNESS_LOGDIR    ?= .
LOGFILE = $(HARNESS_LOGDIR)/upcxx.log
TO_LOG = >> $(LOGFILE) 2>&1

# nobs requires bash
SHELL := /bin/bash

# Paths and options for standard tools
WGET              ?= wget -nv
GZCAT             ?= gzip -cd
TAR               ?= tar
UNZIP             ?= unzip
P7ZIP             ?= 7za
GIT               ?= git


# Logic to extract GASNet's compiler variables.
# nobs does not yet automatically pickup GASNET_CC/CXX for external GASNET
# intentionally ignore the environment here because it is set by harness
# use UPCXX_TEST_ENV to override if necessary.
# We use a sub-shell to avoid pulling in the entire makefile fragment.
# TODO: may want to handle LDFLAGS here as well?
GASNET_VARS = $(shell $(MAKE) --no-print-directory echovars FRAGMENT=$(strip $(UPCXX_CONDUIT)))
FRAGMENT=#empty by default
ifneq ($(FRAGMENT),) # handle build or install dir, use any fragment we can find
-include $(UPCXX_FRAGMENTS)/$(FRAGMENT)-conduit/$(FRAGMENT)-seq.mak
-include $(UPCXX_FRAGMENTS)/$(FRAGMENT)-conduit/$(FRAGMENT)-par.mak
-include $(UPCXX_FRAGMENTS)/include/$(FRAGMENT)-conduit/$(FRAGMENT)-seq.mak
-include $(UPCXX_FRAGMENTS)/include/$(FRAGMENT)-conduit/$(FRAGMENT)-par.mak
echovars: force; @echo "CC='$(GASNET_CC)' CXX='$(GASNET_CXX)'"
.PHONY: echovars
endif

RUNTIME_TARGETS = test/hello_gasnet.cpp test/hello_upcxx.cpp
COMMON_ENV = \
        $(GASNET_VARS) \
        DBGSYM=$(DBGSYM) OPTLEV=$(OPTLEV) ASSERT=$(ASSERT) \
	GASNET="$(NOBS_GASNET)" GASNET_CONDUIT=$(UPCXX_CONDUIT) \
	TERM=dumb PAGER=cat 
DO_ENV = unset $(UNSET_VARS) ; export $(COMMON_ENV) $(UPCXX_TEST_ENV) && \
         export CC="$$CC $(strip $(UPCXX_EXTRA_FLAGS))" CXX="$$CXX $(strip $(UPCXX_EXTRA_FLAGS))"
DO_NOBS = cd $(UPCXX_BLDDIR) && source ./sourceme && $(DO_ENV) && nobs

# Parametrized support for download/unpack
UPCXX_DOWNLOAD ?=# one of "unzip", "p7zip" or "tgz", default to auto-detect from downloaded archive suffix
upcxx_dl_suffix_unzip := .zip
upcxx_dl_suffix_p7zip := .zip
upcxx_dl_suffix_tgz   := .tar.gz
upcxx_dl_suffix=$(upcxx_dl_suffix_$(strip $(UPCXX_DOWNLOAD)))
ifeq ($(strip $(upcxx_dl_suffix)),)
upcxx_dl_suffix := .zip
endif

# XXX: Hard-coded download of fixed tarball for now
UPCXX_URL      ?= https://upc-bugs.lbl.gov/nightly/unlisted/berkeleylab-upcxx-$(UPCXX_GIT_COMMIT)$(upcxx_dl_suffix)
#UPCXX_URL      ?= https://bitbucket.org/$(UPCXX_GIT_REPO)/get/$(UPCXX_GIT_COMMIT)$(upcxx_dl_suffix)

upcxx-download: force
	rm -Rf $(UPCXX_BLDDIR) $(UPCXX_TMPDIR) && mkdir $(UPCXX_TMPDIR)
	cd $(UPCXX_TMPDIR) && $(WGET) $(UPCXX_URL) 2>&1
	cd $(UPCXX_TMPDIR) && cat wget-log 2> /dev/null ; rm -f wget-log ; exit 0
	@UPCXX_TESTDIR=`pwd` && \
	 cd $(UPCXX_TMPDIR) && UPCXX_ARCHIVE=`/bin/ls` && UPCXX_DOWNLOAD="$(strip $(UPCXX_DOWNLOAD))" && \
	 if test -z "$$UPCXX_DOWNLOAD" ; then \
	   case "$$UPCXX_ARCHIVE" in \
	     *.zip)          UPCXX_DOWNLOAD=unzip ;; \
	     *.p7z|*.7z)     UPCXX_DOWNLOAD=p7zip ;; \
	     *.tar.gz|*.tgz) UPCXX_DOWNLOAD=tgz   ;; \
	     *) echo "ERROR: Unknown archive suffix on '$$UPCXX_ARCHIVE': Set UPCXX_DOWNLOAD=(unzip,p7zip,tgz)" ; exit 1; \
	   esac \
	 fi && \
	 echo Fetched $$UPCXX_ARCHIVE : UPCXX_DOWNLOAD=$$UPCXX_DOWNLOAD && \
	 $(MAKE) -f $$UPCXX_TESTDIR/Makefile upcxx-unpack-$$UPCXX_DOWNLOAD UPCXX_ARCHIVE="$$UPCXX_ARCHIVE" && \
	 rm -f $(UPCXX_TMPDIR)/$$UPCXX_ARCHIVE
	mv $(UPCXX_TMPDIR)/* $(UPCXX_BLDDIR) # archive's root dir will vary
	rmdir $(UPCXX_TMPDIR)
# Three ways to unpack the archive:
#  Option 1: "unzip" - .zip w/ unzip
#   This is the favored approach because it gives us the hash and uses a widely available utility.
upcxx-unpack-unzip: force; $(UNZIP) -z $(UPCXX_ARCHIVE) && $(UNZIP) -q $(UPCXX_ARCHIVE)
#  Option 2: "p7zip" - .zip w/ 7za (from p7zip package)
#   This also gives us the hash, but uses a less widely available utility.
#   However, it is sometimes necessary because many unzip installations contain a bug w.r.t. symlinks
upcxx-unpack-p7zip: force; $(P7ZIP) x -bd $(UPCXX_ARCHIVE)
#  Option 3: "tgz" - tar + gzip
#   This is the most portable, but cannot extract the hash unless git is available
upcxx-unpack-tgz:   force
	-$(GZCAT) $(UPCXX_ARCHIVE) | $(GIT) get-tar-commit-id ; exit 0
	$(GZCAT) $(UPCXX_ARCHIVE) | $(TAR) xf -
####
.PHONY: upcxx-download upcxx-unpack-unzip upcxx-unpack-p7zip upcxx-unpack-tgz

# Symlink or simple script that corresponds to
#    RunCmd = ./launcher -np %N %P %A
launcher:
	@UPCXXRUN=$(UPCXX_BLDDIR)/utils/upcxx-run ; \
	 if test -f $$UPCXXRUN ; then set -x ; \
	   rm -f $@ ; \
	   echo '#!/bin/sh' > $@ ; \
	   echo "exec env GASNET_PREFIX='$(UPCXX_LAUNCHERS)' $$UPCXXRUN"' "$$@"' >> $@ ; \
	   chmod +x $@ ; \
	 else \
	   echo "ERROR: $$UPCXXRUN is missing" ; exit 1 ; \
	 fi 

_upcxx: force
	rm -Rf upcxx-built launcher $(LOGFILE)
	$(MAKE) upcxx-download 2>&1 | tee $(LOGFILE)
	@echo UPCXX_GASNET=$(UPCXX_GASNET) NOBS_GASNET=$(NOBS_GASNET) UPCXX_BLDDIR=$(UPCXX_BLDDIR) UPCXX_FRAGMENTS=$(UPCXX_FRAGMENTS) $(TO_LOG)
	@if test -n "$(UPCXX_EXTERNAL_GASNET)" ; then \
	  echo ======== UPCXX_GASNET:external ========; set -x; \
	elif test -n "$(UPCXX_INSTALL_GASNET)" ; then \
	  echo ======== UPCXX_GASNET:install ========; set -x; \
	  $(MAKE) install -C $(TOP_BUILDDIR)/gasnet prefix="$(UPCXX_BLDDIR)/gasnet"; \
	elif test -n "$(UPCXX_TARBALL_GASNET)" ; then \
	  echo ======== UPCXX_GASNET:tarball ========; set -x; \
	  $(MAKE) dist -C $(TOP_BUILDDIR)/gasnet && \
          cp -f $(TOP_BUILDDIR)/gasnet/GASNet-*.tar.gz $(UPCXX_BLDDIR)/gasnet.tar.gz ; \
	else \
	  echo ======== UPCXX_GASNET:builddir ========; set -x; \
	fi $(TO_LOG)
	$(MAKE) launcher $(TO_LOG)
	-echo PATH="$$PATH" $(TO_LOG)
	@-( set -x; $(DO_ENV) ; $$CC  --version ) 2>&1 | egrep -i -v -e error -e warning $(TO_LOG)
	@-( set -x; $(DO_ENV) ; $$CXX --version ) 2>&1 | egrep -i -v -e error -e warning $(TO_LOG)
	@( echo ======== Prebuild UPCXX RT libs ========; \
	  for f in $(RUNTIME_TARGETS); do \
             echo '$(DO_NOBS) exe '$$f ; \
                   $(DO_NOBS) exe $$f || exit 1; \
             echo ; \
          done; \
	) $(TO_LOG)
	@touch upcxx-built
	@echo '#!/bin/sh' > $@ ; chmod +x $@

_upcxx-inst-SEQ:
	$(MAKE) upcxx-inst-gasnet_seq UPCXX_BACKEND=gasnet_seq 2>&1 | tee -a $(LOGFILE)
	@echo '#!/bin/sh' > $@ ; chmod +x $@

_upcxx-inst-PAR:
	$(MAKE) upcxx-inst-gasnet_par UPCXX_BACKEND=gasnet_par 2>&1 | tee -a $(LOGFILE)
	@echo '#!/bin/sh' > $@ ; chmod +x $@

UPCXX_INSTDIR_FULL  := $(UPCXX_INSTDIR)-$(UPCXX_BACKEND)

upcxx-inst-$(UPCXX_BACKEND): upcxx-built
	@echo UPCXX_GASNET=$(UPCXX_GASNET) NOBS_GASNET=$(NOBS_GASNET) UPCXX_BLDDIR=$(UPCXX_BLDDIR) UPCXX_FRAGMENTS=$(UPCXX_FRAGMENTS) UPCXX_BACKEND=$(UPCXX_BACKEND)
	rm -Rf $(UPCXX_INSTDIR_FULL) $@
	@echo ======== UPCXX install $(UPCXX_BACKEND) ========; set -x; \
	 cd $(UPCXX_BLDDIR) && $(DO_ENV) && ./install --single $(UPCXX_INSTDIR_FULL) && \
	 /bin/ls -alR $(UPCXX_INSTDIR_FULL)
	@touch upcxx-install-$(UPCXX_BACKEND)

# Build a single test from the nobs build directory
nobs_test: upcxx-built
	$(DO_NOBS) exe $(TEST_PATH)
	OUTPUT=`$(DO_NOBS) exe $(TEST_PATH)` && cp $$OUTPUT $(TEST_EXE)

# Build a single test from the installed UPCXX directory
inst_test: upcxx-install-$(UPCXX_BACKEND)
	@set -x ; $(DO_ENV) && \
	META_CXX=`$(UPCXX_INSTDIR_FULL)/bin/upcxx-meta CXX` && \
	CXXFLAGS=`$(UPCXX_INSTDIR_FULL)/bin/upcxx-meta CXXFLAGS` && \
	PPFLAGS=`$(UPCXX_INSTDIR_FULL)/bin/upcxx-meta PPFLAGS` && \
	LDFLAGS=`$(UPCXX_INSTDIR_FULL)/bin/upcxx-meta LDFLAGS` && \
	LIBFLAGS=`$(UPCXX_INSTDIR_FULL)/bin/upcxx-meta LIBFLAGS` && \
	$$META_CXX $$CXXFLAGS $$PPFLAGS $$LDFLAGS -o $(TEST_EXE) $(TEST_PATH) $(UPCXX_TEST_FLAGS) $(UPCXX_EXTRA_FLAGS) $$LIBFLAGS

VPATH=$(UPCXX_BLDDIR)/test:$(UPCXX_BLDDIR)/test/uts:$(UPCXX_BLDDIR)/test/regression

# Pattern rule builds any named test with a corresponding cpp in test directory
%-seq %-par :: %.cpp force
	@case "$@" in *-seq) UPCXX_BACKEND=gasnet_seq ;; *-par) UPCXX_BACKEND=gasnet_par ;; esac; set -x ; \
	$(MAKE) nobs_test TEST_PATH=$< TEST_EXE=$@ UPCXX_EXTRA_FLAGS='$(UPCXX_TEST_FLAGS) $(UPCXX_EXTRA_FLAGS)'

# Pattern rule builds example/prog-guide tests
%-seq %-par :: $(UPCXX_BLDDIR)/example/prog-guide/%.cpp force
	@case "$@" in *-seq) UPCXX_BACKEND=gasnet_seq ;; *-par) UPCXX_BACKEND=gasnet_par ;; esac; \
	 if test -n "$(UPCXX_USE_META)"; then set -x ; \
	  $(MAKE) inst_test TEST_PATH=$< TEST_EXE=$@ ; \
	else set -x ; \
	  $(MAKE) nobs_test TEST_PATH=$< TEST_EXE=$@ UPCXX_EXTRA_FLAGS='$(UPCXX_TEST_FLAGS) $(UPCXX_EXTRA_FLAGS) '-DUPCXX_BACKEND=$$UPCXX_BACKEND ; \
	fi

force:

.PHONY: force nobs_test inst_test
