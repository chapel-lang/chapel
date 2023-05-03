# Copyright 2020-2023 Hewlett Packard Enterprise Development LP
# Copyright 2004-2019 Cray Inc.
# Other additional copyright holders may be indicated within.
#
# The entirety of this work is licensed under the Apache License,
# Version 2.0 (the "License"); you may not use this file except
# in compliance with the License.
#
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#
# top-level Chapel Makefile
#

#
# This is the one Makefile that does not/should not include
# $CHPL_HOME/make/Makefile.base.  The reasons are:
#
# (1) it does not need to include that file because it does not rely
# on any of its settings; it only is responsible for cd-ing into
# subdirectories and having them make things (where they should
# include Makefile.base).
#
# (2) including it will actually break the build because there are
# aspects of the CHPL_* environment that we don't know yet.
# Specifically, the third-party-try-opt rule speculatively tries to
# build GMP and RE2 if the user has not expressed a preference, and we
# can't know whether CHPL_GMP/RE2 should be set to 'none' or the
# package until those attempts to build complete.  If Makefile.base is
# included here, it will set them for the make environment based on
# the current state of the world, not the
# post-attempt-to-build-gmp/re2 world.
#

#
# We set this to avoid extraneous printing of Makefile subdirectories
# by default.  Having this un-set will break the Travis builds.
# Normally, Makefile.base sets this for our other Makefiles.
#
MAKEFLAGS = --no-print-directory

export CHPL_MAKE_HOME=$(shell pwd)
export CHPL_MAKE_PYTHON := $(shell $(CHPL_MAKE_HOME)/util/config/find-python.sh)

default: all

all: comprt
	@test -r Makefile.devel && $(MAKE) develall || echo ""

comprt: FORCE
	@$(MAKE) compiler
	@$(MAKE) third-party-try-opt
	@$(MAKE) always-build-test-venv
	@$(MAKE) always-build-chpldoc
	@$(MAKE) runtime
	@$(MAKE) modules

notcompiler: FORCE
	@$(MAKE) third-party-try-opt
	@$(MAKE) always-build-test-venv
	@$(MAKE) runtime
	@$(MAKE) modules

frontend: FORCE
	@echo "Making the frontend compiler library..."
	@cd third-party && $(MAKE) llvm
	@cd third-party && $(MAKE) CHPL_MAKE_HOST_TARGET=--host jemalloc
	@cd compiler && $(MAKE) frontend

compiler: FORCE
	@echo "Making the compiler..."
	@cd third-party && $(MAKE) llvm
	@cd third-party && $(MAKE) CHPL_MAKE_HOST_TARGET=--host jemalloc
	@cd compiler && $(MAKE)

parser: FORCE
	@echo "Making the parser..."
	@cd compiler && $(MAKE) parser

modules: FORCE
	@echo "Making the modules..."
	@cd modules && $(MAKE)

runtime: FORCE
	@echo "Making the runtime..."
	@cd runtime && $(MAKE)

third-party-try-opt: third-party-try-re2 third-party-try-gmp

third-party-try-re2: FORCE
	cd third-party && $(MAKE) try-re2;

third-party-try-gmp: FORCE
	cd third-party && $(MAKE) try-gmp;

third-party-test-venv: FORCE
	@if [ -z "$$CHPL_DONT_BUILD_TEST_VENV" ]; then \
	cd third-party && $(MAKE) test-venv; \
	fi

third-party-chpldoc-venv: FORCE
	@if [ -z "$$CHPL_DONT_BUILD_CHPLDOC_VENV" ]; then \
	cd third-party && $(MAKE) chpldoc-venv; \
	fi

third-party-c2chapel-venv: FORCE
	@if [ -z "$$CHPL_DONT_BUILD_C2CHAPEL_VENV" ]; then \
	cd third-party && $(MAKE) c2chapel-venv; \
	fi

test-venv: third-party-test-venv

chpldoc: third-party-chpldoc-venv
	@cd third-party && $(MAKE) llvm
	cd compiler && $(MAKE) chpldoc
	@cd modules && $(MAKE)
	@test -r Makefile.devel && $(MAKE) man-chpldoc || echo ""

always-build-test-venv: FORCE
	-@if [ -n "$$CHPL_ALWAYS_BUILD_TEST_VENV" ]; then \
	$(MAKE) test-venv; \
	fi

always-build-chpldoc: FORCE
	-@if [ -n "$$CHPL_ALWAYS_BUILD_CHPLDOC" ]; then \
	$(MAKE) chpldoc; \
	fi

chplvis: compiler third-party-fltk FORCE
	cd tools/chplvis && $(MAKE)
	cd tools/chplvis && $(MAKE) install

mason: compiler chpldoc notcompiler FORCE
	cd tools/mason && $(MAKE) && $(MAKE) install

protoc-gen-chpl: chpldoc notcompiler FORCE
	cd tools/protoc-gen-chpl && $(MAKE) && $(MAKE) install

c2chapel: third-party-c2chapel-venv FORCE
	cd tools/c2chapel && $(MAKE)
	cd tools/c2chapel && $(MAKE) install

compile-util-python: FORCE
	@if $(CHPL_MAKE_PYTHON) -m compileall -h > /dev/null 2>&1 ; then \
	  echo "Compiling Python scripts in util/" ; \
	  $(CHPL_MAKE_PYTHON) -m compileall util/config -q ; \
	  $(CHPL_MAKE_PYTHON) -m compileall util/chplenv -q ; \
	  if [ -d third-party/chpl-venv/install/chpldeps ] ; then \
	    echo "Compiling Python scripts in chpl-venv/" ; \
	    $(CHPL_MAKE_PYTHON) -m compileall third-party/chpl-venv/install/chpldeps/ -q ; \
	  fi ; \
	else \
	  echo "Not compiling Python scripts - missing compileall" ; \
	fi

third-party-fltk: FORCE
	cd third-party/fltk && $(MAKE)

clean: FORCE
	cd compiler && $(MAKE) clean
	cd modules && $(MAKE) clean
	cd runtime && $(MAKE) clean
	cd third-party && $(MAKE) clean
	cd tools/chpldoc && $(MAKE) clean
	if [ -e doc/Makefile ]; then cd doc && $(MAKE) clean; fi
	rm -f util/chplenv/*.pyc

cleanall: FORCE
	cd compiler && $(MAKE) cleanall
	cd modules && $(MAKE) cleanall
	cd runtime && $(MAKE) cleanall
	cd third-party && $(MAKE) cleanall
	cd tools/chpldoc && $(MAKE) cleanall
	if [ -e doc/Makefile ]; then cd doc && $(MAKE) cleanall; fi
	rm -f util/chplenv/*.pyc
	rm -rf build

cleandeps: FORCE
	cd compiler && $(MAKE) cleandeps
	cd runtime && $(MAKE) cleandeps

clobber: FORCE
	cd compiler && $(MAKE) clobber
	cd modules && $(MAKE) clobber
	cd runtime && $(MAKE) clobber
	cd third-party && $(MAKE) clobber
	cd tools/chplvis && $(MAKE) clobber
	cd tools/c2chapel && $(MAKE) clobber
	cd tools/mason && $(MAKE) clobber
	-cd tools/protoc-gen-chpl && $(MAKE) clobber
	cd tools/chpldoc && $(MAKE) clobber
	if [ -e doc/Makefile ]; then cd doc && $(MAKE) clobber; fi
	rm -rf bin
	rm -rf lib
	rm -rf build
	rm -f util/chplenv/*.pyc
	rm -f compiler/main/CONFIGURED_PREFIX
# these files might be generated by ./configure
	rm -f configured-chplconfig configured-prefix configured-chpl-home chplconfig

depend:
	@echo "make depend has been deprecated for the time being"

check:
	@+CHPL_HOME=$(CHPL_MAKE_HOME) bash $(CHPL_MAKE_HOME)/util/test/checkChplInstall

check-chpldoc: chpldoc third-party-test-venv
	@bash $(CHPL_MAKE_HOME)/util/test/checkChplDoc

install: comprt
	@bash $(CHPL_MAKE_HOME)/util/buildRelease/install.sh --stage=${DESTDIR}

-include Makefile.devel

FORCE:

# Don't want to try building e.g. GMP and RE2 at the same time
.NOTPARALLEL:
