#
# settings for gnu-compatible platforms
#

#
# Tools
#
CXX = g++
CC = gcc

#
# Flags
#

CFLAGS += -Wall

GNU_GCC_MAJOR_VERSION = $(shell $(CC) -dumpversion | awk '{split($$1,a,"."); printf("%s", a[1]);}')
GNU_GCC_MINOR_VERSION = $(shell $(CC) -dumpversion | awk '{split($$1,a,"."); printf("%s", a[2]);}')

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

CXXFLAGS += -Werror

CXXFLAGS = $(CFLAGS)

ifeq ($(GNU_GCC_MAJOR_VERSION),3)
ifeq ($(GNU_GCC_MINOR_VERSION),4)
CXXFLAGS += -Wno-invalid-offsetof
endif
endif

