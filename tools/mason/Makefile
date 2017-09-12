# Copyright 2004-2017 Cray Inc.
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

ifndef MASON_HOME
export MASON_HOME=$(HOME)
endif

include $(CHPL_MAKE_HOME)/make/Makefile.base

bdir=$(CHPL_BIN_DIR)
link=$(bdir)/mason

TOML=$(CHPL_HOME)/test/modules/packages/Toml/
REGISTRY=https://github.com/chapel-lang/mason-registry

registry = $(MASON_HOME)/.mason/registry/


all: mason


mason:	
	@if [ $(shell $(CHPL_MAKE_HOME)/util/chplenv/chpl_regexp.py) != re2 ]; then \
        echo "CHPL_REGEXP=re2 is required to build mason"; \
        exit 1;\
	fi;
	@mkdir -p $(MASON_HOME)/.mason/src
ifneq ($(wildcard $(registry)),)
	@echo "Removing old registry..."
	@rm -rf $(MASON_HOME)/.mason/registry
endif
	@echo "Registry - $(registry)"
	@git -C $(MASON_HOME)/.mason/ clone $(REGISTRY) registry
	@echo "Building Mason..."
	@chpl mason.chpl -M $(TOML)

install: mason
ifneq ($(wildcard $(link)),)
	@echo "Removing old symbolic link..."
	rm -f $(link)
	@echo
endif
	@echo "Installing symbolic link..."
	@mkdir -p $(bdir)
	@ln -s $(shell pwd)/mason $(link)

update:
	@echo "Re-building Mason..."
	@chpl mason.chpl -M $(TOML)

clean:
	@echo "Removing Mason..."
	rm -f $(shell pwd)/mason
ifneq ($(wildcard $(link)),)
	@echo "Removing old symbolic link..."
	@rm -f $(link)
	@echo
endif


clobber: clean
	@echo "Removing Registry..."
	rm -rf $(MASON_HOME)/.mason/registry