#
# Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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
#
# chplcheck - a Python-based Chapel linter.

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(realpath $(shell pwd)/../..)
endif
ifndef CHPL_MAKE_PYTHON
export CHPL_MAKE_PYTHON := $(shell $(CHPL_MAKE_HOME)/util/config/find-python.sh)
endif

include $(CHPL_MAKE_HOME)/make/Makefile.base
include $(CHPL_MAKE_HOME)/third-party/chpl-venv/Makefile.include

bdir=$(CHPL_BIN_DIR)
link=$(bdir)/chplcheck

all: chplcheck install

chplcheck-venv:
	cd $(CHPL_MAKE_HOME)/third-party && $(MAKE) chplcheck-venv

chplcheck: chplcheck-venv

chplcheck-docs:
	$(CHPL_MAKE_PYTHON) $(CHPL_MAKE_HOME)/doc/util/chplcheck-docs.py \
		-r $(CHPL_MAKE_HOME)/tools/chplcheck/src/rules.py \
		-o $(CHPL_MAKE_HOME)/doc/rst/tools/chplcheck/generated \
		--examples-directory $(CHPL_MAKE_HOME)/tools/chplcheck/examples

clean-chplcheck-docs:
	rm -rf $(CHPL_MAKE_HOME)/doc/rst/tools/chplcheck/generated

clean: clean-pycache
ifneq ($(wildcard $(link)),)
	@echo "Removing old symbolic link..."
	rm -f $(link)
	@echo
endif

cleanall: clean

clobber: clean clean-chplcheck-docs

clean-pycache:
	find $(CHPL_MAKE_HOME)/tools/chplcheck -type d -name __pycache__ -exec rm -rf {} +

$(link): clean
	@echo "Installing chplcheck symbolic link..."
	mkdir -p $(bdir)
	ln -s $(shell pwd)/chplcheck $(link)

install: chplcheck $(link)
