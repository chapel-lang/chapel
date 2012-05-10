RUNTIME_ROOT = ../../..
RUNTIME_SUBDIR = src/mem/dlmalloc

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/$(RUNTIME_ROOT)/..
endif

include $(RUNTIME_ROOT)/make/Makefile.runtime.head
 
MEM_OBJDIR = $(RUNTIME_OBJDIR)

include Makefile.share

TARGETS = $(MEM_COMMON_OBJS) $(DLMALLOC_LIB_DIR)/libdlmalloc.a

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
