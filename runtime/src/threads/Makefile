RUNTIME_ROOT = ../..
RUNTIME_SUBDIR = src/threads

#
# standard header
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.head

#
# include source subdirectories here
#
include $(CHPL_MAKE_THREADS)/Makefile.include

include Makefile.share

TARGETS =

SUBDIRS = $(CHPL_MAKE_THREADS)

include $(RUNTIME_ROOT)/make/Makefile.runtime.subdirrules

FORCE:

#
# standard footer
#
include $(RUNTIME_ROOT)/make/Makefile.runtime.foot
