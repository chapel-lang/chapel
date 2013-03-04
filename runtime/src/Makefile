RUNTIME_ROOT = ..
RUNTIME_SUBDIR = src

ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/$(RUNTIME_ROOT)/..
endif

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

#
# include source subdirectories here
#

COMMON_OBJDIR = $(RUNTIME_OBJDIR)
COMMON_LAUNCHER_OBJDIR = $(LAUNCHER_OBJDIR)
include Makefile.share

ifneq ($(MAKE_LAUNCHER),1)
TARGETS = \
	$(COMMON_OBJS) \
	$(MAIN_OBJS) \

SUBDIRS = \
	comm \
	mem \
	tasks \
	threads \
        timers \
        qio \

else
TARGETS = \
	$(MAIN_LAUNCHER_OBJS) \
	$(COMMON_LAUNCHER_OBJS) \

SUBDIRS = \
	comm \
	launch \

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
