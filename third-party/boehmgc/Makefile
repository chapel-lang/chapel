CHAPEL_ROOT = ../..
CHAPEL_SUBDIR = third-party/boehmgc
include $(CHAPEL_ROOT)/make/Makefile.base


all: $(PLATFORM_DIR)
	@-cp .lastbuild .lastbuild.tmp
	@echo $(PLATFORM) > .lastbuild
	@diff .lastbuild.tmp .lastbuild > /dev/null 2>&1 || ($(MAKE) clean; \
	cd $(GC_DIRNAME); \
	./configure --prefix='$(CURDIR)/$(PLATFORM)' --disable-threads; \
	make; \
	make check; \
	make install)

clean:
	cd $(GC_DIRNAME) && make clean

clobber:
	cd $(GC_DIRNAME) && make clean
	rm -rf ./$(PLATFORM)

