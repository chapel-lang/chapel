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

ifdef DEBUG
CFLAGS += -g -DDEBUG=1
else
CFLAGS += -DNDEBUG=1
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

ONLYCFLAGS += -Werror

ifeq ($(GNU_GCC_MAJOR_VERSION),3)
ifeq ($(GNU_GCC_MINOR_VERSION),4)
ONLYCXXFLAGS += -Wno-invalid-offsetof -Werror
else
endif
else
endif
