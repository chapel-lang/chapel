#
# Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
# c2chapel -- a tool to generate Chapel bindings from a C99 file
#
# TODO: Allow builders to specifiy a local tarball for pyparser
#

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(realpath $(shell pwd)/../..)
endif

include $(CHPL_MAKE_HOME)/make/Makefile.base
include $(CHPL_MAKE_HOME)/third-party/chpl-venv/Makefile.include

bdir=$(CHPL_BIN_DIR)
link=$(bdir)/c2chapel

VERSION=2.17
TAR=release_v$(VERSION).tar.gz

RELEASE=https://github.com/eliben/pycparser/archive/$(TAR)

INSTALL=$(realpath .)/install

# Update wrapper script if this changes
VENV_DIR=$(INSTALL)/venv

FAKES=$(INSTALL)/fakeHeaders

VIRTUAL_ENV=$(shell which virtualenv)
THIRD_PARTY_VENV=$(CHPL_VENV_DIR)/$(CHPL_VENV_VIRTUALENV)

.PHONY: clean clobber c2chapel install venv

all: c2chapel

venv:
ifneq ($(wildcard $(VIRTUAL_ENV)),)
	$(info Using system virtualenv)
else ifneq ($(wildcard $(THIRD_PARTY_VENV)),)
	$(eval VIRTUAL_ENV=PYTHONPATH=$(PIPLIBS) && $(THIRD_PARTY_VENV))
	# Add warning suppression flags
	$(eval VENV_PIP_FLAGS=--no-warn-conflicts --no-warn-script-location)
	$(info Using chpl-venv virtualenv ($(VIRTUAL_ENV)))
else
	$(error Unable to find virtualenv. Either install virtualenv yourself \
  or 'make test-venv' from $$CHPL_HOME)
endif

c2chapel: venv
	mkdir -p $(VENV_DIR)
	mkdir -p $(FAKES)
	cd $(INSTALL) && \
	echo "Fetching release $(VERSION) tarball..." && \
	wget $(RELEASE) -O $(TAR) && \
	tar xzf $(TAR) -C $(FAKES) --strip 3 pycparser-release_v$(VERSION)/utils/fake_libc_include

	./utils/fixFakes.sh $(FAKES) $(PWD)/utils/custom.h

	$(VIRTUAL_ENV) $(VENV_DIR)

	. $(VENV_DIR)/bin/activate && \
	  python $(VENV_DIR)/bin/pip install $(VENV_PIP_FLAGS) $(INSTALL)/$(TAR) && \
	  python $(VENV_DIR)/bin/pip install $(VENV_PIP_FLAGS) $(CHPL_PIP_INSTALL_PARAMS) argparse

check:
	cd test && ./tester.sh

clean:
	rm -rf $(INSTALL)
ifneq ($(wildcard $(link)),)
	@echo "Removing old symbolic link..."
	rm -f $(link)
	@echo
endif

cleanall: clean

clobber: clean

install:
	@echo "Installing c2chapel..."
ifneq ($(wildcard $(link)),)
	@echo "Removing old symbolic link..."
	rm -f $(link)
	@echo
endif
	@echo "Installing symbolic link..."
	mkdir -p $(bdir)
	ln -s $(shell pwd)/c2chapel $(link)
