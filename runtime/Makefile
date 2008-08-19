CHAPEL_ROOT=..
include $(CHAPEL_ROOT)/make/Makefile.base

all: all.helpme

depend: depend.helpme

clean: clean.helpme

clobber: clobber.helpme

%.helpme:
	@$(MAKE) -f Makefile.help MAKE_LAUNCHER=0 $*
ifneq ($(CHPL_MAKE_LAUNCHER),none)
	@$(MAKE) -f Makefile.help MAKE_LAUNCHER=1 $*
endif
