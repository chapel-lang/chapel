RUNTIME_ROOT = ../../..
RUNTIME_SUBDIR = src/regexp/re2

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/$(RUNTIME_ROOT)/..
endif

include $(RUNTIME_ROOT)/make/Makefile.runtime.head
 
REGEXP_OBJDIR = $(RUNTIME_OBJDIR)

include Makefile.share

TARGETS = $(REGEXP_COMMON_OBJS)

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

include $(RUNTIME_ROOT)/make/Makefile.runtime.foot

OTHER_DEPEND_RULES = \
	cd $(THIRD_PARTY_DIR) && $(MAKE) re2
