# Copyright 2004-2020 Cray Inc.
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


COMPILER_ROOT=.
COMPILER_SUBDIR = .

#
# subdirectories and false subdirectory-oriented targets to force recursion
#
SUBDIRS = \
	adt \
	AST \
	backend \
	codegen \
	ifa \
	llvm \
	main \
	optimizations \
	parser \
	passes \
	resolution \
	util \

#
# include standard header for compiler
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head

# Generate tags command, dependent on if Make variable, TAGS == 1
ifeq ($(TAGS), 1)
TAGS_COMMAND=-@(which $(CHPL_TAGS_UTIL) > /dev/null 2>&1 && echo "Updating TAGS..." && $(CHPL_TAGS_UTIL) $(CHPL_TAGS_FLAGS) $(ALL_SRCS) */*.h) || echo "$(CHPL_TAGS_UTIL) not available"
EBROWSE_COMMAND=-@(which ebrowse > /dev/null 2>&1 && echo "Updating BROWSE..." && ebrowse $(ALL_SRCS) */*.h) || echo "ebrowse not available"
endif

#
# the location of the BUILD_VERSION file
#
BUILD_VERSION_FILE = $(COMPILER_ROOT)/main/BUILD_VERSION
# configured prefix file
CONFIGURED_PREFIX_FILE = $(COMPILER_BUILD)/CONFIGURED_PREFIX
# file storing the names of clang, clang++ for CHPL_LLVM=system
# also stores the clang-sysroot-arguments
CLANG_SETTINGS_FILE = $(COMPILER_BUILD)/CLANG_SETTINGS

#
# include source subdirectories here
#
include adt/Makefile.include
include AST/Makefile.include
include backend/Makefile.include
include codegen/Makefile.include
include ifa/Makefile.include
include llvm/Makefile.include
include main/Makefile.include
include optimizations/Makefile.include
include parser/Makefile.include
include passes/Makefile.include
include resolution/Makefile.include
include util/Makefile.include

SRCS =

CHPL_OBJS = \
	$(ADT_OBJS) \
	$(AST_OBJS) \
	$(BACKEND_OBJS) \
	$(CODEGEN_OBJS) \
	$(IFA_OBJS) \
	$(LLVM_OBJS) \
	$(MAIN_OBJS) \
	$(OPTIMIZATIONS_OBJS) \
	$(PARSER_OBJS) \
	$(PASSES_OBJS) \
	$(RESOLUTION_OBJS) \
	$(SYMTAB_OBJS) \
	$(UTIL_OBJS) \

EXECS = $(CHPL) $(CHPLDOC)

PRETARGETS = $(BUILD_VERSION_FILE) $(CONFIGURED_PREFIX_FILE) llvm $(CLANG_SETTINGS_FILE)
TARGETS = $(CHPL) $(CHPL_OLD)

LIBS = -lm

# Set up variables representing paths that will be installed
# and how to fix them (for CLANG_SETTINGS).
# So far, this only needs to fix CHPL_THIRD_PARTY/CHPL_HOME.
ifndef CHPL_MAKE_THIRD_PARTY
CHPL_MAKE_THIRD_PARTY = $(CHPL_MAKE_HOME)/third-party
endif

FIXPATH_CMD := $(CHPL_MAKE_HOME)/util/config/replace-paths.py \
                  --fixpath '$$CHPL_THIRD_PARTY' $(CHPL_MAKE_THIRD_PARTY) \
                  --fixpath '$$CHPL_HOME' $(CHPL_MAKE_HOME)

#
# main rules
#

include $(COMPILER_ROOT)/make/Makefile.compiler.rules


#
# target-based rules
#

CHPL_CONFIG_CHECK_PREFIX = $(CHPL_BIN_DIR)/.built-for
CHPL_CONFIG_CHECK_DIR = $(CHPL_CONFIG_CHECK_PREFIX)/$(CHPL_MAKE_COMPILER_SUBDIR)
CHPL_CONFIG_CHECK = $(CHPL_CONFIG_CHECK_DIR)/built-for

UPDATE_BUILD_VERSION = $(CHPL_MAKE_HOME)/util/devel/updateBuildVersion

CLEAN_TARGS += $(CLANG_SETTINGS_FILE)

$(BUILD_VERSION_FILE): FORCE
	@({ test -e $(CHPL_MAKE_HOME)/.git ; } && \
	test -x $(UPDATE_BUILD_VERSION) && $(UPDATE_BUILD_VERSION) $@ $(CHPL_MAKE_HOME)) || \
	test -r $(BUILD_VERSION_FILE) || (echo '"0"' > $@);

$(CHPL_MAKE_HOME)/configured-prefix:
	echo > $(CHPL_MAKE_HOME)/configured-prefix

$(CONFIGURED_PREFIX_FILE): FORCE $(COMPILER_BUILD) $(CHPL_MAKE_HOME)/configured-prefix
	@echo '"'`cat $(CHPL_MAKE_HOME)/configured-prefix`'"' \ > $(CONFIGURED_PREFIX_FILE).incoming
	@$(CHPL_MAKE_HOME)/util/config/update-if-different $(CONFIGURED_PREFIX_FILE) $(CONFIGURED_PREFIX_FILE).incoming

$(CLANG_SETTINGS_FILE): FORCE $(COMPILER_BUILD)
	@echo '{"'$(CLANG_CC)'","'$(CLANG_CXX)'","'`$(CHPL_MAKE_HOME)/util/config/gather-clang-sysroot-arguments $(CLANG_CC)`'"}' | $(FIXPATH_CMD) > $(CLANG_SETTINGS_FILE).incoming
	@$(CHPL_MAKE_HOME)/util/config/update-if-different $(CLANG_SETTINGS_FILE) $(CLANG_SETTINGS_FILE).incoming

$(CHPL_CONFIG_CHECK): | $(CHPL_BIN_DIR)
	rm -rf $(CHPL_CONFIG_CHECK_PREFIX)
	mkdir -p $(CHPL_CONFIG_CHECK_DIR)
	echo $(CHPL_MAKE_COMPILER_SUBDIR) > $(CHPL_CONFIG_CHECK)


$(CHPL): $(CHPL_OBJS) $(CHPL_CONFIG_CHECK) | $(CHPL_BIN_DIR)
	$(CXX) $(LDFLAGS) -o $@ $(CHPL_OBJS) $(LIBS) $(LLVM_LIBS)
	$(TAGS_COMMAND)
	$(EBROWSE_COMMAND)

$(CHPL_OLD): $(CHPL) $(CHPL_OLD_BIN_DIR)
	cp "$(CHPL_MAKE_HOME)/util/config/old-chpl.sh" "$(CHPL_OLD_BIN_DIR)/chpl"

$(CHPLDOC): $(CHPL)
	rm -f $(CHPLDOC)
	ln -s $(notdir $(CHPL)) $(CHPLDOC)

chpldoc: $(CHPLDOC)

parser: FORCE
	$(MAKE) -C parser parser

$(COMPILER_BUILD):
	mkdir -p $@

$(CHPL_BIN_DIR):
	mkdir -p $@

$(CHPL_OLD_BIN_DIR):
	mkdir -p $@


# third-party pkgs could be automatically built with the below.
#
#THIRD_PARTY_PKGS = $(shell $(CHPL_MAKE_HOME)/util/chplenv/third-party-pkgs --compiler)
# NOTE: If any of these builds fail, we won't stop building
#third-party-pkgs:
#	@for p in $(THIRD_PARTY_PKGS); do \
#		$(MAKE) -C $(THIRD_PARTY_DIR) $$p; \
#	done

llvm: FORCE
	@echo "Making LLVM..."
	@$(MAKE) -C $(THIRD_PARTY_DIR) llvm

#
# include standard footer for compiler
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot

.NOTPARALLEL:
