#
# settings for gnu-compatible platforms
#

#
# Tools
#
CXX = g++
CC = gcc
MAKEDEPEND = $(CXX) -MM -MG


#
# versions
#

GNU_GCC_MAJOR_VERSION = $(shell $(CC) -dumpversion | awk '{split($$1,a,"."); printf("%s", a[1]);}')
GNU_GCC_MINOR_VERSION = $(shell $(CC) -dumpversion | awk '{split($$1,a,"."); printf("%s", a[2]);}')


#
# Flags
#

ifdef CHPL_GCOV
CFLAGS += -fprofile-arcs -ftest-coverage
LDFLAGS += -fprofile-arcs
endif

ifdef DEBUG
CFLAGS += -g
endif

ifdef OPTIMIZE
CFLAGS += -O2
endif

ifdef PROFILE
CFLAGS += -pg
LDFLAGS += -pg
endif

CFLAGS += -Wall


# add error stuff
# in general, can't set -Werror for C++ because of the use of offsetof
# which requires -Wno-invalid-offsetof to get around... :(

ONLYCFLAGS += -Werror

ifeq ($(GNU_GCC_MAJOR_VERSION),3)
ifeq ($(GNU_GCC_MINOR_VERSION),4)
ONLYCXXFLAGS += -Werror
endif
endif
