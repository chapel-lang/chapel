# Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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
link=../../tools/protoc-gen-chpl/protoc-gen-chpl
linkFile=$(bdir)/protoc-gen-chpl

SOURCES=$(wildcard *.cpp)
HEADERS=$(wildcard *.h)
LDFLAGS = $(shell pkg-config --libs protobuf 2>/dev/null) -lprotobuf -lprotoc
CPPFLAGS = $(shell pkg-config --cflags protobuf 2>/dev/null) -I. -std=c++11

ifeq ($(CHPL_MAKE_PLATFORM),darwin)
	PROTOC_LIB_LOC=$(dir $(shell which protoc))../lib
	UPDATE_RPATH = install_name_tool -add_rpath $(PROTOC_LIB_LOC)
else
	# Don't want to accidentally execute protoc-gen-chpl when install_name_tool
	# isn't available, so instead use a noop
	UPDATE_RPATH = touch
endif


all: $(bdir)/protoc-gen-chpl FORCE

protoc-gen-chpl: $(bdir)/protoc-gen-chpl FORCE

install: $(bdir)/protoc-gen-chpl FORCE

$(bdir)/protoc-gen-chpl:	$(SOURCES)	$(HEADERS)
	@echo "Building protoc-gen-chpl.."
	$(CC) $(CPPFLAGS) -o $@ $(SOURCES) $(LDFLAGS) -lstdc++
	$(UPDATE_RPATH) $@

clean:
	@echo "Removing protoc-gen-chpl..."
	rm -f $(bdir)/protoc-gen-chpl

check:
	@echo "checking the compatibility of the plugin with the installed protoc version"
	@echo "syntax = 'proto3';" >> a.proto
	@echo "message A {int32 a = 1;}" >> a.proto
	@protoc --chpl_out=. a.proto || (echo "make check failed $?"; rm -rf a.proto; exit 1)
	@echo "make check passed"
	@rm -rf a.proto
	@rm -rf a.chpl

clobber: clean FORCE

FORCE:
