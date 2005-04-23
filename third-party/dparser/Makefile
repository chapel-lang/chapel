CHAPEL_ROOT = ../..
CHAPEL_SUBDIR = third-party/dparser
include $(CHAPEL_ROOT)/make/Makefile.base

DIRS = \
	$(PLATFORM)/bin \
	$(PLATFORM)/include \
	$(PLATFORM)/lib \
	$(PLATFORM)/man \
	$(PLATFORM)/man/man1

all: $(DIRS)
	@-cp .lastbuild .lastbuild.tmp
	@echo "$(PLATFORM)-gc" > .lastbuild
	@diff .lastbuild.tmp .lastbuild > /dev/null 2>&1 || ($(MAKE) clean && cd $(D_PARSER_DIRNAME) && $(MAKE) PREFIX=../$(PLATFORM) GCROOT=../../boehmgc/$(PLATFORM) all install)

nogc: $(DIRS)
	@-cp .lastbuild .lastbuild.tmp
	@echo "$(PLATFORM)-nogc" > .lastbuild
	@diff .lastbuild.tmp .lastbuild > /dev/null 2>&1 || ($(MAKE) clean && cd $(D_PARSER_DIRNAME) && $(MAKE) PREFIX=../$(PLATFORM) GCROOT=../../boehmgc/$(PLATFORM) D_USE_GC= all install)

clean:
	cd $(D_PARSER_DIRNAME) && $(MAKE) clean

clobber:
	cd $(D_PARSER_DIRNAME) && $(MAKE) clean
	rm -rf ./$(PLATFORM)

$(PLATFORM)/bin: $(PLATFORM_DIR)
	mkdir -p ./$@
	@touch ./$@

$(PLATFORM)/include: $(PLATFORM_DIR)
	mkdir -p ./$@
	@touch ./$@

$(PLATFORM)/lib: $(PLATFORM_DIR)
	mkdir -p ./$@
	@touch ./$@

$(PLATFORM)/man: $(PLATFORM_DIR)
	mkdir -p ./$@
	@touch ./$@

$(PLATFORM)/man/man1: $(PLATFORM_DIR) $(PLATFORM)/man
	mkdir -p ./$@
	@touch ./$@

