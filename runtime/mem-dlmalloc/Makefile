RUNTIME_ROOT = ..
RUNTIME_SUBDIR = mem-dlmalloc

include $(RUNTIME_ROOT)/make/Makefile.runtime.head
 
MEM_OBJDIR = $(RUNTIME_OBJDIR)

include Makefile.share

TARGETS = $(MEM_COMMON_OBJS) $(MEM_DLMALLOC_LIB_DIR)/libdlmalloc.a

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
