CHAPEL_ROOT = ../..
include $(CHAPEL_ROOT)/make/Makefile.base

all:
	mkdir -p $(PLATFORM)
	mkdir -p $(PLATFORM)/bin
	mkdir -p $(PLATFORM)/include
	mkdir -p $(PLATFORM)/lib
	mkdir -p $(PLATFORM)/man
	mkdir -p $(PLATFORM)/man/man1
	cd $(D_PARSER_DIRNAME) && $(MAKE) PREFIX=../$(PLATFORM) GCROOT=../../boehmgc/$(PLATFORM) all install

clean:
	cd $(D_PARSER_DIRNAME) && $(MAKE) clean
	rm -rf $(PLATFORM)/*
