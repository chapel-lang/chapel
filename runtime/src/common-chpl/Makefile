RUNTIME_ROOT = ../..
RUNTIME_SUBDIR = src/common-chpl

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

COMMON_CHPL_OBJDIR = $(RUNTIME_OBJDIR)
COMMON_CHPL_LAUNCHER_OBJDIR = $(LAUNCHER_OBJDIR)
include Makefile.share

ifneq ($(MAKE_LAUNCHER),1)
TARGETS = \
	$(COMMON_CHPL_OBJS) \

else
TARGETS = \
	$(COMMON_CHPL_LAUNCHER_OBJS) \

endif

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot

$(RUNTIME_OBJ_DIR)/chpl_rt_utils.o: chpl_rt_utils.c $(RUNTIME_OBJ_DIR_STAMP) $(RUNTIME_INCLUDE_ROOT)/chpl_rt_utils.h $(RUNTIME_INCLUDE_ROOT)/stdchpl.h
	$(CC) -c $(RUNTIME_GEN_CFLAGS) $(RUNTIME_INCLS) -o $@ $<

$(RUNTIME_INCLUDE_ROOT)/chpl_rt_utils.h: chpl_rt_utils.h
	cp $< $@
	touch -r $< $@

chpl_rt_utils.c chpl_rt_utils.h: chpl_rt_utils.chpl $(CHPL) $(CHAPEL_ROOT)/modules/internal/*.chpl
	export CHPL_HOME && CHPL_HOME=$(CHAPEL_ROOT) && $(CHPL) $(RUNTIME_CHPL_FLAGS) $<

OTHER_DEPEND_RULES = \
	@test -r $(RUNTIME_INCLUDE_ROOT)/chpl_rt_utils.h || (echo "/* PLACEHOLDER HEADER */\n\#include \"stdchpl_rt_utils.h\"" > $(RUNTIME_INCLUDE_ROOT)/chpl_rt_utils.h; touch chpl_rt_utils.chpl); \
	test -r chpl_rt_utils.c || (echo "/* PLACEHOLDER CODE */\n\#include \"chpl_rt_utils.h\"" > chpl_rt_utils.c;  touch chpl_rt_utils.chpl); \
	$(CMAKEDEPEND) $(DEPEND_CFLAGS) $(DEPEND_INCLS) chpl_rt_utils.c | sed s/chpl_rt_utils\\\.o/\\\$(DEPEND_DIR_SAFESLASH)\\\/chpl_rt_utils\\\.o/ >> $(SVN_DEPENDS).tmp