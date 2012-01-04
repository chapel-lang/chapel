CHAPEL_ROOT=..
include $(CHAPEL_ROOT)/make/Makefile.base

all: third-party-pkgs all.helpme

depend: third-party-pkgs depend.helpme

clean: clean.helpme

cleandeps: cleandeps.helpme

clobber: clobber.helpme

%.helpme:
	@$(MAKE) -f Makefile.help MAKE_LAUNCHER=0 CHPL_MAKE_RULE=$* $*
ifneq ($(CHPL_MAKE_LAUNCHER),none)
	@$(MAKE) -f Makefile.help MAKE_LAUNCHER=1 CHPL_MAKE_RULE=$* $*
endif

THIRD_PARTY_PKGS = $(shell $(CHAPEL_ROOT)/util/chplenv/third-party-pkgs)
# NOTE: If any of these builds fail, we won't stop building
third-party-pkgs:
	@for p in $(THIRD_PARTY_PKGS); do \
		$(MAKE) -C $(THIRD_PARTY_DIR) $$p; \
	done

