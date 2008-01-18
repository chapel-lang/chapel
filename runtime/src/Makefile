RUNTIME_ROOT = ..
RUNTIME_SUBDIR = src

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

COMMON_OBJDIR = $(OBJ_SUBDIR)/threads-$(CHPL_MAKE_THREADS)/comm-$(CHPL_MAKE_COMM)
include Makefile.share

TARGETS = $(COMMON_OBJS) $(MAIN_OBJS)

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

FORCE:

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
	$(CC) -c -g $(RUNTIME_CFLAGS) -o $@ $<
