RUNTIME_ROOT = ../..
RUNTIME_SUBDIR = src/timers

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

#
# include source subdirectories here
#
include $(CHPL_MAKE_TIMERS)/Makefile.include

include Makefile.share

TARGETS =

SUBDIRS = $(CHPL_MAKE_TIMERS)

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

FORCE:

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
