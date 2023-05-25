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

#
# settings for gnu-compatible platforms
#

#
# Tools
#
CXX = $(CHPL_MAKE_CXX) # normally g++
CC = $(CHPL_MAKE_CC)    # normally gcc

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

include $(CHPL_MAKE_HOME)/make/compiler/Makefile.sanitizers
CFLAGS += $(SANITIZER_CFLAGS)
CXXFLAGS += $(SANITIZER_CFLAGS)
LDFLAGS += $(SANITIZER_LDFLAGS)
OPT_CFLAGS += $(SANITIZER_OPT_CFLAGS)
GEN_CFLAGS += $(SANITIZER_CFLAGS)
GEN_LFLAGS += $(SANITIZER_LDFLAGS)

#
# Flags for compiler, runtime, and generated code
#
# User can provide:
#   CPPFLAGS - C PreProcessor flags
#   CFLAGS   - C flags
#   CXXFLAGS - C++ flags
#   LDFLAGS  - ld (linker) flags
#
# We set
#  COMP_CFLAGS, COMP_CXXFLAGS,       (compiling C/C++ code in compiler/)
#  RUNTIME_CFLAGS, RUNTIME_CXXFLAGS  (compiling C/C++ code in runtime/)
# in a way that respects the above user-provide-able variables.
#
# We set
#  GEN_CFLAGS and GEN_LFLAGS etc ignoring the above variables
#  since adjustments to these should be provided on the chpl command line.
COMP_CFLAGS = $(CPPFLAGS) $(CFLAGS)
COMP_CXXFLAGS = $(CPPFLAGS) $(CXXFLAGS)
# Appended after COMP_CXXFLAGS when compiling parser/lexer
COMP_CXXFLAGS_NONCHPL = -Wno-error

RUNTIME_CFLAGS = $(CPPFLAGS) $(CFLAGS)
RUNTIME_CXXFLAGS = $(CPPFLAGS) $(CXXFLAGS)

GEN_STATIC_FLAG = -static
GEN_DYNAMIC_FLAG =
LIB_STATIC_FLAG =
LIB_DYNAMIC_FLAG = -shared
SHARED_LIB_CFLAGS = -fPIC

# Set the target architecture for optimization
ifneq ($(CHPL_MAKE_TARGET_CPU), none)
ifneq ($(CHPL_MAKE_TARGET_CPU), unknown)
ifneq ($(CHPL_MAKE_TARGET_CPU_FLAG), none)
SPECIALIZE_CFLAGS = -m$(CHPL_MAKE_TARGET_CPU_FLAG)=$(CHPL_MAKE_TARGET_BACKEND_CPU)
endif
endif
endif

# Set flag for lax or IEEE floating point
FAST_FLOAT_GEN_CFLAGS = -ffast-math
IEEE_FLOAT_GEN_CFLAGS = -fno-fast-math

ifeq ($(CHPL_MAKE_PLATFORM), darwin)
# build 64-bit binaries when on a 64-bit capable PowerPC
ARCH := $(shell test -x /usr/bin/machine -a `/usr/bin/machine` = ppc970 && echo -arch ppc64)
GEN_CFLAGS += $(ARCH)
GEN_LFLAGS += $(ARCH)
endif

#ensure that 64 bit binaries are made on AIX
ifeq ($(CHPL_MAKE_PLATFORM), aix)
GEN_CFLAGS += -maix64
RUNTIME_CFLAGS += -maix64
GEN_CFLAGS += -maix64
COMP_GEN_LFLAGS += -maix64
endif

#
# query gcc version
#
ifndef GNU_GCC_MAJOR_VERSION
export GNU_GCC_MAJOR_VERSION := $(shell $(CC) -dumpversion | awk '{split($$1,a,"."); printf("%s", a[1]);}')
endif
ifndef GNU_GCC_MINOR_VERSION
export GNU_GCC_MINOR_VERSION := $(shell $(CC) -dumpversion | awk '{split($$1,a,"."); printf("%s", a[2]);}')
endif
ifndef GNU_GPP_MAJOR_VERSION
export GNU_GPP_MAJOR_VERSION := $(shell $(CXX) -dumpversion | awk '{split($$1,a,"."); printf("%s", a[1]);}')
endif
ifndef GNU_GPP_MINOR_VERSION
export GNU_GPP_MINOR_VERSION := $(shell $(CXX) -dumpversion | awk '{split($$1,a,"."); printf("%s", a[2]);}')
endif
ifndef GNU_GPP_SUPPORTS_MISSING_DECLS
export GNU_GPP_SUPPORTS_MISSING_DECLS := $(shell test $(GNU_GPP_MAJOR_VERSION) -lt 4 || (test $(GNU_GPP_MAJOR_VERSION) -eq 4 && test $(GNU_GPP_MINOR_VERSION) -le 2); echo "$$?")
endif
ifndef GNU_GCC_SUPPORTS_STRICT_OVERFLOW
export GNU_GCC_SUPPORTS_STRICT_OVERFLOW := $(shell test $(GNU_GCC_MAJOR_VERSION) -lt 4 || (test $(GNU_GCC_MAJOR_VERSION) -eq 4 && test $(GNU_GCC_MINOR_VERSION) -le 2); echo "$$?")
endif

#
# If the compiler's default C version is less than C99, force C99 mode.
#
# If the default C version is at least C11, force the C++ version to
# be at least C++11 to match.
#
DEF_C_VER := $(shell echo __STDC_VERSION__ | $(CC) -E -x c - | sed -e '/^\#/d' -e 's/L$$//' -e 's/__STDC_VERSION__/0/')
DEF_CXX_VER := $(shell echo __cplusplus | $(CXX) -E -x c++ - | sed -e '/^\#/d' -e 's/L$$//' -e 's/__cplusplus/0/')
C_STD := $(shell test $(DEF_C_VER) -lt 199901 && echo -std=gnu99)
CXX_STD := $(shell test $(DEF_C_VER) -ge 201112 -a $(DEF_CXX_VER) -lt 201103 && echo -std=gnu++11)

# CXX11_STD is the flag to select C++11, blank for compilers that
# don't know how to do that
# Also, if a compiler uses C++11 or newer by default, CXX11_STD will be blank.
CXX11_STD := $(shell test $(DEF_CXX_VER) -lt 201103 && echo -std=gnu++11)
CXX14_STD := $(shell test $(DEF_CXX_VER) -lt 201402 && echo -std=gnu++14)

ifeq ($(GNU_GPP_MAJOR_VERSION),4)
  CXX_STD   := -std=gnu++11
  CXX11_STD := -std=gnu++11
endif

COMP_CFLAGS += $(C_STD)
ifneq ($(CHPL_MAKE_LLVM_VERSION),16)
COMP_CXXFLAGS += $(CXX14_STD)
else
# get the C++ standard flag from CMake
endif
RUNTIME_CFLAGS += $(C_STD)
RUNTIME_CXXFLAGS += $(CXX_STD)
GEN_CFLAGS += $(C_STD)

#
# Flags for turning on warnings for C++/C code
#
# On Ubuntu, gcc complains about multiline comments in some versions
# of Clang header files.
#
WARN_COMMONFLAGS = -Wall -Werror -Wpointer-arith -Wwrite-strings -Wno-strict-aliasing
WARN_CXXFLAGS = $(WARN_COMMONFLAGS) -Wno-comment -Wmissing-braces
WARN_CFLAGS = $(WARN_COMMONFLAGS) -Wmissing-prototypes -Wstrict-prototypes -Wmissing-format-attribute
WARN_GEN_CFLAGS = $(WARN_CFLAGS)
SQUASH_WARN_GEN_CFLAGS = -Wno-unused -Wno-uninitialized

ifeq ($(shell test $(GNU_GPP_MAJOR_VERSION) -gt 5; echo "$$?"),0)
# We'd like to know about missing overrides but don't let it
# abort the build since there might be some in LLVM headers.
WARN_CXXFLAGS += -Wsuggest-override -Wno-error=suggest-override
endif

#
# Don't warn for signed pointer issues (ex. c_ptr(c_char) )
#
ifeq ($(shell test $(GNU_GCC_MAJOR_VERSION) -lt 4; echo "$$?"),1)
SQUASH_WARN_GEN_CFLAGS += -Wno-pointer-sign
endif

#
# Don't warn/error for incompatible pointer types (see
# https://github.com/chapel-lang/chapel/issues/7983)
#
ifeq ($(shell test $(GNU_GCC_MAJOR_VERSION) -lt 5; echo "$$?"),1)
SQUASH_WARN_GEN_CFLAGS += -Wno-incompatible-pointer-types
endif

ifeq ($(shell test $(GNU_GCC_MAJOR_VERSION) -ge 5; echo "$$?"),0)
SQUASH_WARN_GEN_CFLAGS += -Wno-misleading-indentation
endif

#
# Don't warn/error for tautological compares (ex. x == x)
#
ifeq ($(shell test $(GNU_GCC_MAJOR_VERSION) -lt 6; echo "$$?"),1)
SQUASH_WARN_GEN_CFLAGS += -Wno-tautological-compare
endif

#
# Avoid false positive warnings about string overflows and memcpys
# using strings.  Over time, we've seen this error with a variety of
# gcc compiler versions (7, 9, 10) in a variety of settings, seemingly
# related to our conversions from int64 to uint64, where gcc is
# nervous that we're using a negative or too-large value.  We have
# pretty tight bounds checking for these cases when checks are on, and
# haven't caught real issues as a result of these flags that I'm aware
# of, so let's squash the warnings to avoid false positives.
#
ifeq ($(shell test $(GNU_GCC_MAJOR_VERSION) -gt 7; echo "$$?"),0)
SQUASH_WARN_GEN_CFLAGS += -Wno-stringop-overflow -Wno-array-bounds
endif

#
# This is similar to the use of -Wno-stringop-overflow just above, but
# addressing a complaint that started showing up in gcc 13.1 about our
# use of memmove() to implement local communications.  Given that we
# haven't seen problems in practice and run testing with asan, I'm
# squashing as in the previous case.
#
ifeq ($(shell test $(GNU_GCC_MAJOR_VERSION) -ge 13; echo "$$?"),0)
SQUASH_WARN_GEN_CFLAGS += -Wno-stringop-overread
endif

#
# Disable ipa-clone for gcc 7.  This optimization seemed to cause
# a multi-locale lulesh regression that was fixed in gcc 8
#
ifeq ($(shell test $(GNU_GCC_MAJOR_VERSION) -eq 7; echo "$$?"),0)
OPT_CFLAGS += -fno-ipa-cp-clone
endif

#
# Avoid false positives for allocation size, memcpy, and string and
# unchecked bounded formatted conversion (snprintf()) truncation.
# Note that we use -Walloc-size-larger-than=SIZE_MAX instead of
# `-Wno-alloc-size-larger-than` since that did not exist in gcc 8.
#
ifeq ($(shell test $(GNU_GPP_MAJOR_VERSION) -gt 7; echo "$$?"),0)
WARN_CXXFLAGS += -Walloc-size-larger-than=18446744073709551615
WARN_CXXFLAGS += -Wno-stringop-truncation -Wno-format-truncation
endif

ifeq ($(shell test $(GNU_GCC_MAJOR_VERSION) -gt 7; echo "$$?"),0)
WARN_CFLAGS += -Walloc-size-larger-than=18446744073709551615
WARN_CFLAGS += -Wno-stringop-truncation -Wno-format-truncation
SQUASH_WARN_GEN_CFLAGS += -Walloc-size-larger-than=18446744073709551615 -Wno-restrict
endif

#
# Avoid false positive warnings about class member access and string overflows.
# The string overflow false positives occur in runtime code unlike gcc 7.
# Also avoid false positives for array bounds and comments.
#
ifeq ($(shell test $(GNU_GPP_MAJOR_VERSION) -eq 8; echo "$$?"),0)
WARN_CXXFLAGS += -Wno-class-memaccess
endif

ifeq ($(shell test $(GNU_GCC_MAJOR_VERSION) -eq 8; echo "$$?"),0)
RUNTIME_CFLAGS += -Wno-stringop-overflow
SQUASH_WARN_GEN_CFLAGS += -Wno-array-bounds
endif

#
# Avoid errors about insufficient initializer lifetimes because they
# occur in LLVM headers.  We would like to know when this occurs,
# though, so don't turn off the warning; just don't let it abort the
# build.
#
ifeq ($(shell test $(GNU_GPP_MAJOR_VERSION) -eq 9; echo "$$?"),0)
WARN_CXXFLAGS += -Wno-error=init-list-lifetime
endif

#
# Avoid errors about uninitialized memory because they occur in LLVM headers
# (should be fixed in LLVM 15 though).
# We would like to know when this occurs, though, so don't turn off
# the warning; just don't let it abort the build.
#
ifeq ($(shell test $(GNU_GPP_MAJOR_VERSION) -eq 12; echo "$$?"),0)
WARN_CXXFLAGS += -Wno-error=uninitialized
endif

#
# Avoid errors about dependent template name because they occur in LLVM headers
# (should be fixed in LLVM 15 though).
# We would like to know when this occurs, though, so don't turn off
# the warning; just don't let it abort the build.
#
ifeq ($(shell test $(GNU_GPP_MAJOR_VERSION) -eq 12; echo "$$?"),0)
WARN_CXXFLAGS += -Wno-error=missing-template-keyword
endif

#
# Avoid errors like '&expression' will never be NULL
# because they occur in LLVM headers.
# We would like to know when this occurs, though, so don't turn off
# the warning; just don't let it abort the build.
#
ifeq ($(shell test $(GNU_GPP_MAJOR_VERSION) -eq 12; echo "$$?"),0)
WARN_CXXFLAGS += -Wno-error=address
endif


#
# Avoid false positive warnings about use-after free with realloc
# that occur in GCC 12.
#
ifeq ($(shell test $(GNU_GPP_MAJOR_VERSION) -eq 12; echo "$$?"),0)
WARN_CXXFLAGS += -Wno-use-after-free
endif

#
# 2016/03/28: Help to protect the Chapel compiler from a partially
# characterized GCC optimizer regression when the compiler is being
# compiled with gcc 5.X.
#
# 2017-06-14: Regression apparently fixed since gcc 5.X.  Turning
# off VRP interferes with operation of gcc 7, especially static
# analysis.  The test below was "-ge 5", now changing it to "-eq 5".
#
# Note that 0 means "SUCCESS" rather than "false".
ifeq ($(shell test $(GNU_GPP_MAJOR_VERSION) -eq 5; echo "$$?"),0)

ifeq ($(OPTIMIZE),1)
COMP_CFLAGS += -fno-tree-vrp
COMP_CXXFLAGS += -fno-tree-vrp
endif

endif


ifeq ($(GNU_GPP_SUPPORTS_MISSING_DECLS),1)
WARN_CXXFLAGS += -Wmissing-declarations
else
WARN_CFLAGS += -Wmissing-declarations
endif

ifeq ($(GNU_GCC_SUPPORTS_STRICT_OVERFLOW),1)
# -Wno-strict-overflow is needed only because the way we code range iteration
# (ChapelRangeBase.chpl:793) generates code which can overflow.
SQUASH_WARN_GEN_CFLAGS += -Wno-strict-overflow
# -fstrict-overflow was introduced in GCC 4.2 and is on by default.  When on,
# it allows the compiler to assume that integer sums will not overflow, which
#  can change the programs runtime behavior (when -O2 or greater is tossed).
endif

#
# compiler warnings settings
#
ifeq ($(WARNINGS), 1)
COMP_CFLAGS += $(WARN_CFLAGS)
COMP_CXXFLAGS += $(WARN_CXXFLAGS)
RUNTIME_CFLAGS += $(WARN_CFLAGS) -Wno-char-subscripts
RUNTIME_CXXFLAGS += $(WARN_CXXFLAGS)
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
CHPL_GASNET_MORE_CFLAGS = -Wno-strict-prototypes -Wno-missing-prototypes
endif

ifdef CHPL_COMM_DEBUG
CHPL_GASNET_MORE_CFLAGS += -O0 -UNDEBUG
CHPL_GASNET_MORE_GEN_CFLAGS += -Wno-uninitialized
endif


#
# Look for the libmvec.a static library
#
#
# If found, and static linking is used, then
# it will be included in the generated Makefile LIBS line
ifneq ($(WANT_LIBMVEC), no)
  # Try known locations.
  ifeq ($(CHPL_MAKE_TARGET_PLATFORM), linux64)
    ifeq ($(shell test -e /usr/lib64/libmvec.a; echo "$$?"), 0)
      FOUND_LIBMVEC = yes
    else ifeq ($(shell test -e /usr/lib/x86_64-linux-gnu/libmvec.a; echo "$$?"), 0)
      FOUND_LIBMVEC = yes
    endif
  else ifeq ($(CHPL_MAKE_TARGET_PLATFORM), linux32)
    ifeq ($(shell test -e /usr/libx32/libmvec.a; echo "$$?"), 0)
      FOUND_LIBMVEC = yes
    endif
  endif

  ifeq ($(FOUND_LIBMVEC), yes)
    LIBMVEC = -lmvec
  endif
endif
