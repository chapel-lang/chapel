MAKEFLAGS = --no-print-directory
CHPL_MAKE=$(MAKE) -f Makefile.help

default:
	@echo "Speculatively trying to build re2"
	@if [[ -z "$$CHPL_REGEXP" ]]; then $(CHPL_MAKE) third-party-try-re2; fi
	@$(CHPL_MAKE)

%:
	@$(CHPL_MAKE) $<
