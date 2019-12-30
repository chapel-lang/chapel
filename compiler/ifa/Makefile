# Copyright 2004-2020 Cray Inc.
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
COMPILER_SUBDIR = ifa

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head


IFA_OBJDIR = $(OBJ_SUBDIR)
include Makefile.share


TARGETS = $(IFA_OBJS)


include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules


MAKE_PRIMS = ./make_prims/$(CHPL_MAKE_PLATFORM)/make_prims$(EXE_SUFFIX)

#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot
