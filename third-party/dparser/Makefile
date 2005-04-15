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
	@cp .lastbuild .lastbuild.tmp
	@echo "gc" > .lastbuild
	@diff .lastbuild.tmp .lastbuild > /dev/null 2>&1 || $(MAKE) clean
	cd $(D_PARSER_DIRNAME) && $(MAKE) PREFIX=../$(PLATFORM) GCROOT=../../boehmgc/$(PLATFORM) all install

nogc: $(DIRS)
	@cp .lastbuild .lastbuild.tmp
	@echo "nogc" > .lastbuild
	@diff .lastbuild.tmp .lastbuild > /dev/null 2>&1 || $(MAKE) clean
	cd $(D_PARSER_DIRNAME) && $(MAKE) PREFIX=../$(PLATFORM) GCROOT=../../boehmgc/$(PLATFORM) D_USE_GC= all install

clean:
	cd $(D_PARSER_DIRNAME) && $(MAKE) clean

clobber:
	cd $(D_PARSER_DIRNAME) && $(MAKE) clean
	rm -rf ./$(PLATFORM)

$(PLATFORM)/bin:
	mkdir -p ./$@

$(PLATFORM)/include:
	mkdir -p ./$@

$(PLATFORM)/lib:
	mkdir -p ./$@

$(PLATFORM)/man:
	mkdir -p ./$@

$(PLATFORM)/man/man1:
	mkdir -p ./$@

