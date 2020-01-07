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

RUNTIME_ROOT = ..
RUNTIME_SUBDIR = src

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/$(RUNTIME_ROOT)/..
endif

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

#
# include source subdirectories here
#

COMMON_OBJDIR = $(RUNTIME_OBJDIR)
COMMON_LAUNCHER_OBJDIR = $(LAUNCHER_OBJDIR)
include Makefile.share

ifneq ($(MAKE_LAUNCHER),1)
TARGETS = \
	$(COMMON_OBJS) \
	$(MAIN_OBJS) \
	$(MAIN_MLI_OBJS) \
	$(RUNTIME_MALLOC_OBJS) \

SUBDIRS = \
	comm \
	mem \
	tasks \
	threads \
	timers \
	topo \
	qio \

else
TARGETS = \
	$(MAIN_LAUNCHER_OBJS) \
	$(COMMON_LAUNCHER_OBJS) \
	$(ONLY_LAUNCHER_OBJS)

SUBDIRS = \
	comm \
	launch \

endif

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot

#
# turn on debugging for gdb.c to help make debugging test uniform
# for developers and users
#
$(RUNTIME_OBJ_DIR)/gdb.o: gdb.c $(RUNTIME_OBJ_DIR_STAMP)
	@if [ `grep "chplrt.h" $< | wc -l` -ne 1 ]; then echo "PROBLEM:  $< does not include 'chplrt.h'."; exit 1; fi
	$(CC) -c -g $(RUNTIME_CFLAGS) $(RUNTIME_INCLS) -o $@ $<
