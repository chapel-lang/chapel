# Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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
# Makefile: builds Chapel compiler
#

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/..
endif

CMAKE ?= cmake
COMPILER_ROOT=.
COMPILER_SUBDIR = .

# disable -MMD etc because cmake already specifies it
NO_DEPEND = 1

#
# include standard header for compiler
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head

ALL_HEADERS= */*.h ../frontend/include/chpl/*/*.h

# Generate tags command, dependent on if Make variable, TAGS == 1
ifeq ($(TAGS), 1)
TAGS_COMMAND=-@(which $(CHPL_TAGS_UTIL) > /dev/null 2>&1 && echo "Updating TAGS..." && $(CHPL_TAGS_UTIL) $(CHPL_TAGS_FLAGS) $(ALL_SRCS) $(ALL_HEADERS)) || echo "$(CHPL_TAGS_UTIL) not available"
EBROWSE_COMMAND=-@(which ebrowse > /dev/null 2>&1 && echo "Updating BROWSE..." && ebrowse $(ALL_SRCS) $(ALL_HEADERS)) || echo "ebrowse not available"
endif

#
# include source subdirectories here
# include these subdirs so ETAGS builds tags for .cpp files too
include ../frontend/lib/immediates/Makefile.include
include ../frontend/lib/parsing/Makefile.include
include ../frontend/lib/framework/Makefile.include
include ../frontend/lib/resolution/Makefile.include
include ../frontend/lib/types/Makefile.include
include ../frontend/lib/uast/Makefile.include
include ../frontend/lib/util/Makefile.include

include adt/Makefile.include
include AST/Makefile.include
include backend/Makefile.include
include codegen/Makefile.include
include llvm/Makefile.include
include main/Makefile.include
include optimizations/Makefile.include
include parser/Makefile.include
include passes/Makefile.include
include resolution/Makefile.include
include util/Makefile.include


TARGETS = $(CHPL)

LIBS = -lm

#
# main rules
#

all: $(PRETARGETS) $(MAKEALLSUBDIRS) echocompilerdir $(TARGETS)

clean: FORCE $(CLEANSUBDIRS) echocompilerdir
	@cd $(COMPILER_BUILD) && $(CMAKE) --build . --target clean
	rm -f $(CLEAN_TARGS)

cleanall: $(CLEANALLSUBDIRS) echocompilerdir
	rm -rf $(CLEANALL_TARGS)

cleandeps: $(CLEANSUBDIRDEPS) echocompilerdir
	rm -f $(DEPENDS)

clobber: $(CLOBBERSUBDIRS) echocompilerdir
	rm -rf ./$(CLOBBER_TARGS)


#
# target-based rules
#

CHPL_CONFIG_CHECK_PREFIX = $(CHPL_BIN_DIR)/.built-for
CHPL_CONFIG_CHECK_DIR = $(CHPL_CONFIG_CHECK_PREFIX)/$(CHPL_MAKE_COMPILER_SUBDIR)
CHPL_CONFIG_CHECK = $(CHPL_CONFIG_CHECK_DIR)/built-for

$(CHPL_MAKE_HOME)/configured-prefix:
	echo > $(CHPL_MAKE_HOME)/configured-prefix

$(CONFIGURED_PREFIX_FILE): FORCE $(COMPILER_BUILD) $(CHPL_MAKE_HOME)/configured-prefix
	@echo '"'`cat $(CHPL_MAKE_HOME)/configured-prefix`'"' \ > $(CONFIGURED_PREFIX_FILE).incoming
	@$(CHPL_MAKE_PYTHON) $(CHPL_MAKE_HOME)/util/config/update-if-different --update $(CONFIGURED_PREFIX_FILE) $(CONFIGURED_PREFIX_FILE).incoming

$(CHPL_CONFIG_CHECK): | $(CHPL_BIN_DIR)
	rm -rf $(CHPL_CONFIG_CHECK_PREFIX)
	mkdir -p $(CHPL_CONFIG_CHECK_DIR)
	echo $(CHPL_MAKE_COMPILER_SUBDIR) > $(CHPL_CONFIG_CHECK)


COMPILER_LIB_DIR = $(CHPL_MAKE_HOME)/lib/compiler/$(CHPL_MAKE_HOST_BIN_SUBDIR)
CMAKE_FLAGS = -DCMAKE_C_COMPILER=$(CHPL_MAKE_HOST_CC) -DCMAKE_CXX_COMPILER=$(CHPL_MAKE_HOST_CXX) -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=$(CHPL_BIN_DIR) -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=$(COMPILER_LIB_DIR) -DCHPL_CXX_FLAGS="$(COMP_CXXFLAGS)" -DCHPL_LD_FLAGS="$(LDFLAGS)"
ifeq ($(shell test -e $(CHPL_MAKE_HOME)/configured-prefix; echo "$$?"), 0)
	CMAKE_FLAGS += -DINSTALLATION_MODE=prefix -DCMAKE_INSTALL_PREFIX=$(shell cat $(CHPL_MAKE_HOME)/configured-prefix)
else ifeq ($(shell test -e $(CHPL_MAKE_HOME)/configured-chpl-home; echo "$$?"), 0)
	CMAKE_FLAGS += -DINSTALLATION_MODE=home -DCMAKE_INSTALL_PREFIX=$(shell cat $(CHPL_MAKE_HOME)/configured-chpl-home)
endif


$(CHPL): FORCE $(CHPL_CONFIG_CHECK) $(COMPILER_BUILD) | $(CHPL_BIN_DIR)
	@cd $(COMPILER_BUILD) && $(CMAKE) $(CHPL_MAKE_HOME) $(CMAKE_FLAGS) && $(MAKE) chpl
	$(TAGS_COMMAND)
	$(EBROWSE_COMMAND)

MAKEALLCHPLDOCSUBDIRS = $(CHPLDOC_SUBDIRS:%=%.makedir)

$(CHPLDOC): FORCE $(MAKEALLCHPLDOCSUBDIRS) $(CHPLDOC_OBJS) | $(CHPL_BIN_DIR) $(CHPL)
	@cd $(COMPILER_BUILD) && $(CMAKE) $(CHPL_MAKE_HOME) $(CMAKE_FLAGS) && $(MAKE) chpldoc

chpldoc: $(CHPLDOC)

$(COMPILER_BUILD):
	mkdir -p $@

$(CHPL_BIN_DIR):
	mkdir -p $@

install-chpl-chpldoc: $(CHPL) $(CHPLDOC)
	@cd $(COMPILER_BUILD) && $(CMAKE) $(CHPL_MAKE_HOME) $(CMAKE_FLAGS) && $(MAKE) install

#
# include standard footer for compiler
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot

.NOTPARALLEL:
