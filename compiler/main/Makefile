COMPILER_ROOT = ..
COMPILER_SUBDIR = main

#
# standard header
#
include $(COMPILER_ROOT)/make/Makefile.compiler.head

MAIN_OBJDIR = $(PLATFORM)
include Makefile.share

TARGETS = COPYRIGHT \
	LICENSE \
	$(MAIN_OBJS)

include $(COMPILER_ROOT)/make/Makefile.compiler.subdirrules

FORCE:

#
# standard footer
#
include $(COMPILER_ROOT)/make/Makefile.compiler.foot

LICENSE: ../../LICENSE
	cat $< | sed s/\"/\\\\\"/g | sed s/\^/\"/g | sed s/$$/\\\\n\"/g > $@

COPYRIGHT: ../../COPYRIGHT
	cat $< | sed s/\"/\\\\\"/g | sed s/\^/\"/g | sed s/\$$/\\\\n\"/g > $@
