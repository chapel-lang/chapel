RUNTIME_ROOT = ..
RUNTIME_SUBDIR = src

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

COMMON_OBJDIR = $(RUNTIME_OBJDIR)
COMMON_LAUNCHER_OBJDIR = $(LAUNCHER_OBJDIR)
include Makefile.share

ifneq ($(MAKE_LAUNCHER),1)
TARGETS = \
	$(COMMON_OBJS) \
	$(MAIN_OBJS) \

else
TARGETS = \
	$(MAIN_LAUNCHER_OBJS) \
	$(COMMON_LAUNCHER_OBJS) \

endif

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot

#
# turn on debugging for gdb.c to help make debugging test uniform
# for developers and users
#
$(RUNTIME_OBJ_DIR)/gdb.o: gdb.c $(RUNTIME_OBJ_DIR_STAMP)
	@if [ `grep "chplrt.h" $< | wc -l` -ne 1 ]; then echo "PROBLEM:  $< does not include 'chplrt.h'."; exit 1; fi
	$(CC) -c -g $(RUNTIME_CFLAGS) $(RUNTIME_INCLS) -o $@ $<

$(RUNTIME_OBJ_DIR)/chapel_code.o: chapel_code.c $(RUNTIME_OBJ_DIR_STAMP) chapel_code.h ../include/stdchpl.h
	$(CC) -c $(RUNTIME_GEN_CFLAGS) $(RUNTIME_INCLS) -o $@ $<

chapel_code.c chapel_code.h: chapel_code.chpl $(CHPL)
	export CHPL_HOME && CHPL_HOME=$(CHAPEL_ROOT) && $(CHPL) $(RUNTIME_CHPL_FLAGS) $<

OTHER_DEPEND_RULES = \
	@test -r chapel_code.h || (echo "/* PLACEHOLDER HEADER */\n\#include \"stdchplrt.h\"" > chapel_code.h; touch chapel_code.chpl); \
	test -r chapel_code.c || (echo "/* PLACEHOLDER CODE */\n\#include \"chapel_code.h\"" > chapel_code.c;  touch chapel_code.chpl); \
	$(CMAKEDEPEND) $(DEPEND_CFLAGS) $(DEPEND_INCLS) chapel_code.c | sed s/chapel_code\\\.o/\\\$(DEPEND_DIR_SAFESLASH)\\\/chapel_code\\\.o/ >> $(SVN_DEPENDS).tmp