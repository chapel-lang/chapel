CHAPEL_ROOT = ../..
CHAPEL_SUBDIR = third-party/boehmgc
include $(CHAPEL_ROOT)/make/Makefile.base


all: $(PLATFORM_DIR)
	cd $(GC_DIRNAME); \
	./configure --prefix='$(CURDIR)/$(PLATFORM)' --disable-threads; \
	make; \
	make check; \
	make install

clean:
	cd $(GC_DIRNAME) && make clean
	rm -rf ./$(PLATFORM)/*

clobber:
	rm -rf ./$(PLATFORM)

