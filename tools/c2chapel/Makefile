#
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
#
#
# c2chapel -- a tool to generate Chapel bindings from a C99 file
#

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(realpath $(shell pwd)/../..)
endif

include $(CHPL_MAKE_HOME)/make/Makefile.base
include $(CHPL_MAKE_HOME)/third-party/chpl-venv/Makefile.include

bdir=$(CHPL_BIN_DIR)
link=$(bdir)/c2chapel

# Note, this version is used only for the fake headers,
# and it should match third-party/chpl-venv/c2chapel-requirements.txt
C2CHAPEL_PYCPARSER_VERSION?=3.00

# This can be set to either a URL or a local tarball.
C2CHAPEL_PYCPARSER_TARBALL?=https://github.com/eliben/pycparser/archive/release_v$(C2CHAPEL_PYCPARSER_VERSION).tar.gz

INSTALL=$(realpath .)/install

# Update wrapper script if this changes
VENV_DIR=$(INSTALL)/venv

FAKES=$(INSTALL)/fakeHeaders

.PHONY: clean clobber c2chapel c2chapel-venv install venv

all: c2chapel install

c2chapel-venv:
	cd ../../third-party && $(MAKE) c2chapel-venv

c2chapel: c2chapel-venv $(FAKES)

$(FAKES):
	mkdir -p $(FAKES)
	if [ -f "$(C2CHAPEL_PYCPARSER_TARBALL)" ]; then \
		echo "Using local pycparser tarball $(C2CHAPEL_PYCPARSER_TARBALL) for fake headers..."; \
		tarball="$(C2CHAPEL_PYCPARSER_TARBALL)"; \
	else \
		echo "Fetching pycparser release $(C2CHAPEL_PYCPARSER_VERSION) tarball for fake headers..."; \
		tarball="$(INSTALL)/pycparser-release_v$(C2CHAPEL_PYCPARSER_VERSION).tar.gz"; \
		$(CHPL_MAKE_PYTHON) -c "import sys, urllib.request; urllib.request.urlretrieve(sys.argv[1], sys.argv[2])" \
			"$(C2CHAPEL_PYCPARSER_TARBALL)" "$$tarball" || exit; \
	fi; \
	source_dir="$(INSTALL)/pycparser-source"; \
	rm -rf "$$source_dir"; \
	mkdir -p "$$source_dir"; \
	tar xzf "$$tarball" -C "$$source_dir"; \
	fake_headers=$$(find "$$source_dir" -type d -path '*/utils/fake_libc_include' -print -quit); \
	if [ -z "$$fake_headers" ]; then \
		echo "Could not find utils/fake_libc_include in $$tarball" >&2; \
		rm -rf "$$source_dir"; \
		exit 1; \
	fi; \
	cp -R "$$fake_headers"/. "$(FAKES)"; \
	rm -rf "$$source_dir"

	./utils/fixFakes.sh $(FAKES) $(PWD)/utils/custom.h

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


$(link):
	@echo "Installing c2chapel..."
ifneq ($(wildcard $(link)),)
	@echo "Removing old symbolic link..."
	rm -f $(link)
	@echo
endif
	@echo "Installing symbolic link..."
	mkdir -p $(bdir)
	ln -s $(shell pwd)/c2chapel $(link)

install: c2chapel $(link)
