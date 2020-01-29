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
# Makefile: builds stuff related to make_prims
#

COMPILER_ROOT = ../..
COMPILER_SUBDIR = ifa/make_prims

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head


#
# make_prims-specific variables
#

MAKE_PRIMS = make_prims$(EXE_SUFFIX)
MAKE_PRIMS_SRCS = \
	make_prims.cpp
MAKE_PRIMS_OBJS = $(MAKE_PRIMS_SRCS:%.cpp=$(OBJ_SUBDIR)/%.o)


#
# standard subdirectory variables
#

SRCS = $(MAKE_PRIMS_SRCS)
EXECS = $(MAKE_PRIMS)


#
# rules
#

TARGETS = $(MAKE_PRIMS)

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules


#
# target-specific rules
#

$(MAKE_PRIMS): $(CHPL_MAKE_PLATFORM)/$(MAKE_PRIMS)
	cp -f $< $@

$(CHPL_MAKE_PLATFORM)/$(MAKE_PRIMS): $(MAKE_PRIMS_OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)


#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
