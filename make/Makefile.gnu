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

CFLAGS += -Wall -Wno-invalid-offsetof

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

