RUNTIME_ROOT = ..
RUNTIME_SUBDIR = mem-lea

include $(RUNTIME_ROOT)/make/Makefile.runtime.head
 
MEM_OBJDIR = $(RUNTIME_OBJDIR)
MEM_LAUNCHER_OBJ_DIR = $(LAUNCHER_OBJDIR)

include Makefile.share

ifneq ($(MAKE_LAUNCHER),1)
TARGETS = $(MEM_COMMON_OBJS) $(MEM_LEA_LIB_DIR)/libdlmalloc.a
else
TARGETS = $(MEM_LAUNCHER_OBJS) $(MEM_LEA_LIB_DIR)/libdlmalloc.a
endif

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
