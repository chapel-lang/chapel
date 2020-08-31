# Copyright 2020 Hewlett Packard Enterprise Development LP
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

unexport CHPL_MAKE_CHPLENV_CACHE

bdir=$(CHPL_BIN_DIR)
link=../../tools/mason/mason
linkFile=$(bdir)/mason

MASON_SOURCES=*.chpl

all: $(bdir)/mason FORCE

mason: $(bdir)/mason FORCE

install: $(bdir)/mason FORCE

$(bdir)/mason: $(MASON_SOURCES)
	@if [ $(shell $(CHPL_MAKE_HOME)/util/chplenv/chpl_regexp.py) != re2 ]; then \
        echo "CHPL_REGEXP=re2 is required to build mason"; \
        exit 1;\
	fi;
	@echo "Building Mason..."
	@chplBinDir=$(bdir) ./buildMason

clean: FORCE
	@echo "Removing Mason..."
	rm -f $(bdir)/mason

clobber: clean FORCE

FORCE:
