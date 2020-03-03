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

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

COMPILER_ROOT = ..
COMPILER_SUBDIR = util
COMP_CXXFLAGS += -I$(COMPILER_ROOT)/ifa

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head

UTIL_OBJDIR = $(OBJ_SUBDIR)
include Makefile.share

TARGETS = $(UTIL_OBJS)

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules

FORCE:

#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot

#
# Compile this with -g so that this symbol is accessible from
# within gdb whether or not we're compiling the whole thing in
# debug mode.
#
$(OBJ_SUBDIR)/tmpdirname.o: tmpdirname.cpp $(OBJ_SUBDIR_MADE)
	$(CXX) -g -c $(COMP_CXXFLAGS) -o $@ $<
