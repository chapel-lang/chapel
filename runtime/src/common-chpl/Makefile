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

$(RUNTIME_OBJ_DIR)/chpl_rt_utils.o: $(RUNTIME_OBJ_DIR)/chpl_rt_utils.c ../../include/chpl_rt_utils_static.h $(RUNTIME_OBJ_DIR_STAMP) $(RUNTIME_OBJ_DIR)/chpl_rt_utils.h $(RUNTIME_INCLUDE_ROOT)/stdchpl.h
	$(CC) -c $(RUNTIME_GEN_CFLAGS) $(RUNTIME_INCLS) -o $@ $<

$(RUNTIME_OBJ_DIR)/chpl_rt_utils.c $(RUNTIME_OBJ_DIR)/chpl_rt_utils.h: chpl_rt_utils.chpl $(CHPL) $(CHAPEL_ROOT)/modules/internal/*.chpl $(RUNTIME_OBJ_DIR_STAMP)
	export CHPL_HOME && CHPL_HOME=$(CHAPEL_ROOT) && $(CHPL) $(RUNTIME_CHPL_FLAGS) ../../include/chpl_rt_utils_static.h $<
	perl -e 'while (<>) { if(/static (static )+/) { s/static //g; print "static int32_t chpl_localeID; static " . $$_; } else { print $$_; } }' chpl_rt_utils.c > $(RUNTIME_OBJ_DIR)/chpl_rt_utils.c
	rm chpl_rt_utils.c
	mv chpl_rt_utils.h $(RUNTIME_OBJ_DIR)

OTHER_DEPEND_RULES = \
	@test -r $(RUNTIME_OBJ_DIR)/chpl_rt_utils.h || (echo "/* PLACEHOLDER HEADER */\n\#include \"stdchpl_rt_utils.h\"" > $(RUNTIME_OBJ_DIR)/chpl_rt_utils.h; touch chpl_rt_utils.chpl); \
	test -r $(RUNTIME_OBJ_DIR)/chpl_rt_utils.c || (echo "/* PLACEHOLDER CODE */\n\#include \"chpl_rt_utils.h\"" > $(RUNTIME_OBJ_DIR)/chpl_rt_utils.c;  touch chpl_rt_utils.chpl); \
	$(CMAKEDEPEND) $(DEPEND_CFLAGS) $(DEPEND_INCLS) $(RUNTIME_OBJ_DIR)/chpl_rt_utils.c | sed s/chpl_rt_utils\\\.o/\\\$(DEPEND_DIR_SAFESLASH)\\\/chpl_rt_utils\\\.o/ >> $(SVN_DEPENDS).tmp
