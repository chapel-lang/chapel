RUNTIME_ROOT = ..
RUNTIME_SUBDIR = mem-hmm

include $(RUNTIME_ROOT)/make/Makefile.runtime.head
 
MEM_OBJDIR = $(RUNTIME_OBJDIR)
MEM_LAUNCHER_OBJ_DIR = $(LAUNCHER_OBJDIR)

include Makefile.share

ifneq ($(MAKE_LAUNCHER),1)
TARGETS = $(MEM_COMMON_OBJS) $(MEM_HMM_LIB_DIR)/libhmm.a
else
TARGETS = $(MEM_LAUNCHER_OBJS) $(MEM_HMM_LIB_DIR)/libhmm.a
endif


include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
