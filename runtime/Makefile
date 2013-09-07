ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/..
endif

include $(CHPL_MAKE_HOME)/make/Makefile.base

all: third-party-pkgs all.helpme

depend:

clean: clean.helpme

cleanall: cleanall.helpme

cleandeps: cleandeps.helpme

clobber: clobber.helpme

%.helpme:
	@$(MAKE) -f Makefile.help MAKE_LAUNCHER=0 CHPL_MAKE_RULE=$* $*
ifneq ($(CHPL_MAKE_LAUNCHER),none)
	@$(MAKE) -f Makefile.help MAKE_LAUNCHER=1 CHPL_MAKE_RULE=$* $*
endif

THIRD_PARTY_PKGS = $(shell $(CHPL_MAKE_HOME)/util/chplenv/third-party-pkgs)
# NOTE: If any of these builds fail, we won't stop building
third-party-pkgs:
	@for p in $(THIRD_PARTY_PKGS); do \
		$(MAKE) -C $(THIRD_PARTY_DIR) $$p; \
	done

