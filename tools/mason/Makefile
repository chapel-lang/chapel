# Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(realpath $(shell pwd)/../..)
endif

include $(CHPL_MAKE_HOME)/make/Makefile.base
CHPL_DEVELOPER ?= 0
include $(CHPL_MAKE_HOME)/make/Makefile.isTrue
ifneq (, $(call isTrue, $(CHPL_DEVELOPER)))
DEBUG=1
else
DEBUG=0
endif

unexport CHPL_MAKE_CHPLENV_CACHE

override CHPL_COMPILER ?= $(CHPL_BIN_DIR)/chpl
outputFile=$(CHPL_BIN_DIR)/mason

MASON_SOURCES=*.chpl

all: $(outputFile) FORCE

mason: $(outputFile) FORCE

install: $(outputFile) FORCE

$(outputFile): $(MASON_SOURCES) buildMason deps
	@echo "Building Mason..."
	@CHPL_COMPILER=$(CHPL_COMPILER) outputFile=$(outputFile) DEBUG=$(DEBUG) ./buildMason

deps:
	./pullMasonDeps

tarball-prep: deps
	@echo "Preparing Mason for tarball..."
	@rm third-party/masonDeps.txt

clean: FORCE
	@echo "Removing Mason..."
	rm -f $(outputFile)

clobber: clean FORCE

FORCE:
