#
# settings for gnu-compatible platforms
#

#
# Tools
#
CC = g++


#
# Flags
#

CFLAGS += -Wall

GNU_GCC_MAJOR_VERSION = $(shell $(CC) -dumpversion | awk '{split($$1,a,"."); printf("%s", a[1]);}')
GNU_GCC_MINOR_VERSION = $(shell $(CC) -dumpversion | awk '{split($$1,a,"."); printf("%s", a[2]);}')

ifeq ($(GNU_GCC_MAJOR_VERSION),3)
ifeq ($(GNU_GCC_MINOR_VERSION),4)
CFLAGS += -Wno-invalid-offsetof -Werror
endif
endif

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

