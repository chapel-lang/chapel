# Copyright 2004-2015 Cray Inc.
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
# settings for gnu-compatible platforms
#

#
# Tools
#
CXX = g++
CC = gcc

RANLIB = ranlib


#
# General Flags
#

DEBUG_CFLAGS = -g
DEPEND_CFLAGS = -MMD -MP
OPT_CFLAGS = -O3
PROFILE_CFLAGS = -pg
PROFILE_LFLAGS = -pg

ifdef CHPL_GCOV
CFLAGS += -fprofile-arcs -ftest-coverage
LDFLAGS += -fprofile-arcs
endif

#
# Flags for compiler, runtime, and generated code
#
COMP_CFLAGS = $(CFLAGS)
COMP_CFLAGS_NONCHPL = -Wno-error
RUNTIME_CFLAGS = -std=c99 $(CFLAGS)
RUNTIME_GEN_CFLAGS = $(RUNTIME_CFLAGS)
RUNTIME_CXXFLAGS = $(CFLAGS)
RUNTIME_GEN_CXXFLAGS = $(RUNTIME_CXXFLAGS)
GEN_CFLAGS = -std=c99
GEN_STATIC_FLAG = -static
GEN_DYNAMIC_FLAG =
LIB_STATIC_FLAG = 
LIB_DYNAMIC_FLAG = -shared
SHARED_LIB_CFLAGS = -fPIC

# Set the target architecture for optimization
ifneq ($(CHPL_MAKE_TARGET_ARCH), none)
ifneq ($(CHPL_MAKE_TARGET_ARCH), unknown)
SPECIALIZE_CFLAGS = -march=$(CHPL_MAKE_TARGET_ARCH)
endif
endif

# Set flag for lax or IEEE floating point
FAST_FLOAT_GEN_CFLAGS = -ffast-math
IEEE_FLOAT_GEN_CFLAGS = -fno-fast-math

ifeq ($(CHPL_MAKE_PLATFORM), darwin)
# build 64-bit binaries when on a 64-bit capable PowerPC
ARCH := $(shell test -x /usr/bin/machine -a `/usr/bin/machine` = ppc970 && echo -arch ppc64)
# -Wa,-q passes control over from the horribly outdated version of as to clang's
#  as this is needed to set things like -march=native or else gcc will generate
#  instructions as can't actually assemble.
RUNTIME_CFLAGS += $(ARCH) -Wa,-q
RUNTIME_CXXFLAGS += $(ARCH) -Wa,-q
# the -D_POSIX_C_SOURCE flag prevents nonstandard functions from polluting the global name space
GEN_CFLAGS += -D_POSIX_C_SOURCE $(ARCH) -Wa,-q
GEN_LFLAGS += $(ARCH)
endif

#ensure that 64 bit binaries are made on AIX
ifeq ($(CHPL_MAKE_PLATFORM), aix)
GEN_CFLAGS += -maix64
RUNTIME_CFLAGS += -maix64
RUNTIME_GEN_CFLAGS += -maix64
GEN_CFLAGS += -maix64
COMP_GEN_LFLAGS += -maix64
endif

#
# query gcc version
#
ifndef GNU_GPP_MAJOR_VERSION
export GNU_GPP_MAJOR_VERSION = $(shell $(CXX) -dumpversion | awk '{split($$1,a,"."); printf("%s", a[1]);}')
endif
ifndef GNU_GPP_MINOR_VERSION
export GNU_GPP_MINOR_VERSION = $(shell $(CXX) -dumpversion | awk '{split($$1,a,"."); printf("%s", a[2]);}')
endif
ifndef GNU_GPP_SUPPORTS_MISSING_DECLS
export GNU_GPP_SUPPORTS_MISSING_DECLS = $(shell test $(GNU_GPP_MAJOR_VERSION) -lt 4 || (test $(GNU_GPP_MAJOR_VERSION) -eq 4 && test $(GNU_GPP_MINOR_VERSION) -le 2); echo "$$?")
endif
ifndef GNU_GPP_SUPPORTS_STRICT_OVERFLOW
export GNU_GPP_SUPPORTS_STRICT_OVERFLOW = $(shell test $(GNU_GPP_MAJOR_VERSION) -lt 4 || (test $(GNU_GPP_MAJOR_VERSION) -eq 4 && test $(GNU_GPP_MINOR_VERSION) -le 2); echo "$$?")
endif

#
# Flags for turning on warnings for C++/C code
#
WARN_CXXFLAGS = -Wall -Werror -Wpointer-arith -Wwrite-strings -Wno-strict-aliasing
# decl-after-stmt for non c99 compilers. See commit message 21665
WARN_CFLAGS = $(WARN_CXXFLAGS) -Wmissing-prototypes -Wstrict-prototypes -Wnested-externs -Wdeclaration-after-statement -Wmissing-format-attribute
WARN_GEN_CFLAGS = $(WARN_CFLAGS) -Wno-unused -Wno-uninitialized

#
# Don't warn for signed pointer issues (ex. c_ptr(c_char) )
#
ifeq ($(shell test $(GNU_GPP_MAJOR_VERSION) -lt 4; echo "$$?"),1)
WARN_GEN_CFLAGS += -Wno-pointer-sign
else

ifeq ($(GNU_GPP_SUPPORTS_MISSING_DECLS),1)
WARN_CXXFLAGS += -Wmissing-declarations
else
WARN_CFLAGS += -Wmissing-declarations
endif

ifeq ($(GNU_GPP_SUPPORTS_STRICT_OVERFLOW),1)
# -Wno-strict-overflow is needed only because the way we code range iteration
# (ChapelRangeBase.chpl:793) generates code which can overflow.  
GEN_CFLAGS += -Wno-strict-overflow
# -fstrict-overflow was introduced in GCC 4.2 and is on by default.  When on,
# it allows the compiler to assume that integer sums will not overflow, which
#  can change the programs runtime behavior (when -O2 or greater is tossed).
endif

#
# compiler warnings settings
#
ifeq ($(WARNINGS), 1)
COMP_CFLAGS += $(WARN_CXXFLAGS)
RUNTIME_CFLAGS += $(WARN_CFLAGS) -Wno-char-subscripts
RUNTIME_CXXFLAGS += $(WARN_CXXFLAGS)
RUNTIME_CXXFLAGS += $(WARN_CXXFLAGS)
RUNTIME_GEN_CFLAGS += -Wno-unused
RUNTIME_GEN_CXXFLAGS += -Wno-unused
#WARN_GEN_CFLAGS += -Wunreachable-code
# GEN_CFLAGS gets warnings added via WARN_GEN_CFLAGS in comp-generated Makefile


#
# When compiling GASNet...
#
# -Wno-missing-prototypes must be used because
# gasneti_linkconfig_idiotcheck isn't prototyped before it's used (boo)
# and is static, so we can't prototype it for them (awwww).
#
# -Wno-strict-prototypes has to be used because most GASNet prototypes
# aren't strict.
#
# -Wno-unused has to be used due to _dummy_checkalign variables in
# -gasnet_atomicops.h
#
CHPL_GASNET_MORE_CFLAGS = -Wno-strict-prototypes -Wno-missing-prototypes
ifndef CHPL_COMM_DEBUG
CHPL_GASNET_MORE_CFLAGS += -Wno-unused
endif
endif
ifdef CHPL_COMM_DEBUG
CHPL_GASNET_MORE_CFLAGS += -O0
CHPL_GASNET_MORE_GEN_CFLAGS += -Wno-uninitialized
endif
