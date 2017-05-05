#
# Copyright 2017 Cray Inc.
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

link=$(CHPL_BIN_DIR)/c2chapel

VERSION=2.17
TAR=release_v$(VERSION).tar.gz

RELEASE=https://github.com/eliben/pycparser/archive/$(TAR)

INSTALL=$(realpath .)/install
VENV=$(INSTALL)/venv
FAKES=$(INSTALL)/fakeHeaders

.PHONY: clean clobber c2chapel install

all: c2chapel

c2chapel:
	mkdir -p $(VENV)
	mkdir -p $(FAKES)
	cd $(INSTALL) && \
	echo "Fetching release $(VERSION) tarball..." && \
	wget $(RELEASE) -O $(TAR) && \
	tar xzf $(TAR) -C $(FAKES) --strip 3 pycparser-release_v$(VERSION)/utils/fake_libc_include

	./utils/fixFakes.sh $(FAKES) $(PWD)/utils/custom.h

	virtualenv $(VENV)
	. $(VENV)/bin/activate && \
	  pip install $(INSTALL)/$(TAR) && \
	  pip install argparse

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
	ln -s $(shell pwd)/c2chapel $(link)
