CHAPEL_ROOT = ../..
CHAPEL_SUBDIR = third-party/dparser
include $(CHAPEL_ROOT)/make/Makefile.base

all: $(PLATFORM_DIR)
	mkdir -p ./$(PLATFORM)/bin
	mkdir -p ./$(PLATFORM)/include
	mkdir -p ./$(PLATFORM)/lib
	mkdir -p ./$(PLATFORM)/man
	mkdir -p ./$(PLATFORM)/man/man1
	cd $(D_PARSER_DIRNAME) && $(MAKE) PREFIX=../$(PLATFORM) GCROOT=../../boehmgc/$(PLATFORM) all install

clean:
	cd $(D_PARSER_DIRNAME) && $(MAKE) clean

clobber:
	cd $(D_PARSER_DIRNAME) && $(MAKE) clean
	rm -rf ./$(PLATFORM)
