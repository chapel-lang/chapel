#
# Copyright 2024 Hewlett Packard Enterprise Development LP
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
# chpl-language-server - a Python-based Chapel language server.

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(realpath $(shell pwd)/../..)
endif

include $(CHPL_MAKE_HOME)/make/Makefile.base
include $(CHPL_MAKE_HOME)/third-party/chpl-venv/Makefile.include

bdir=$(CHPL_BIN_DIR)
link=$(bdir)/chpl-language-server

all: chpl-language-server install

chpl-language-server-venv:
	cd $(CHPL_MAKE_HOME)/third-party && $(MAKE) chpl-language-server-venv

chpl-language-server: chpl-language-server-venv

clean:
ifneq ($(wildcard $(link)),)
	@echo "Removing old symbolic link..."
	rm -f $(link)
	@echo
endif

cleanall: clean

clobber: clean

$(link): clean
	@echo "Installing chpl-language-server symbolic link..."
	mkdir -p $(bdir)
	ln -s $(shell pwd)/chpl-language-server $(link)

install: chpl-language-server $(link)
