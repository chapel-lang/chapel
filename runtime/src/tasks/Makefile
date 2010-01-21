RUNTIME_ROOT = ../..
RUNTIME_SUBDIR = src/tasks

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

#
# include source subdirectories here
#
include $(CHPL_MAKE_TASKS)/Makefile.include

include Makefile.share

TARGETS =

SUBDIRS = $(CHPL_MAKE_TASKS)

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

FORCE:

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
