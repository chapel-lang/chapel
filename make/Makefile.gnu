#
# settings for gnu-compatible platforms
#

#
# Tools
#
CXX = g++
CC = gcc
MAKEDEPEND = $(CXX) -MM -MG
CMAKEDEPEND = $(CC) -MM -MG


#
# General Flags
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

ifdef CHPL_DEVELOPER
CFLAGS += -Wall -Werror
endif


#
# Flags for compiler, runtime, and generated code
#

COMP_CFLAGS = $(CFLAGS)
RUNTIME_CFLAGS = $(CFLAGS)
GEN_CFLAGS = 

ifdef CHPL_DEVELOPER
RUNTIME_CFLAGS += -Wmissing-declarations -Wmissing-prototypes -Wstrict-prototypes
GEN_CFLAGS += -Wall -Werror -Wmissing-declarations -Wmissing-prototypes -Wstrict-prototypes
endif
