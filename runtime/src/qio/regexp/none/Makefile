RUNTIME_ROOT = ../../../..
RUNTIME_SUBDIR = src/qio/regexp/none

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/$(RUNTIME_ROOT)/..
endif

include $(RUNTIME_ROOT)/make/Makefile.runtime.head
 
REGEXP_OBJDIR = $(RUNTIME_OBJDIR)

include Makefile.share

TARGETS = $(REGEXP_OBJS)

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

include $(RUNTIME_ROOT)/make/Makefile.runtime.foot

