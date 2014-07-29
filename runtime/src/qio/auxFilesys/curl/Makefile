RUNTIME_ROOT = ../../../..
RUNTIME_SUBDIR = src/qio/auxFilesys/curl

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/$(RUNTIME_ROOT)/..
endif

include $(RUNTIME_ROOT)/make/Makefile.runtime.head
 
AUXFS_CURL_OBJDIR = $(RUNTIME_OBJDIR)

include Makefile.share

TARGETS = $(AUXFS_CURL_OBJS)

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
