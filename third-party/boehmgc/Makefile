CHAPEL_ROOT = ../..
CHAPEL_SUBDIR = third-party/boehmgc
include $(CHAPEL_ROOT)/make/Makefile.base


all: $(PLATFORM_DIR) .lastbuild
	@echo $(PLATFORM) > .lastbuild.tmp
	@diff .lastbuild.tmp .lastbuild > /dev/null 2>&1 || $(MAKE) all-force

all-force: $(PLATFORM_DIR) FORCE
	$(MAKE) clean
	cd $(GC_DIRNAME); \
	./configure --prefix='$(CURDIR)/$(PLATFORM)' --disable-threads; \
	make; \
	make check; \
	make install
	cp .lastbuild.tmp .lastbuild

clean:
	cd $(GC_DIRNAME) && make clean

clobber:
	cd $(GC_DIRNAME) && make clean
	rm -rf ./$(PLATFORM)

.lastbuild:
	@echo "never-build" > .lastbuild

FORCE:
