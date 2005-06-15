CHAPEL_ROOT = ../..
CHAPEL_SUBDIR = third-party/boehmgc
include $(CHAPEL_ROOT)/make/Makefile.base


all: $(PLATFORM_DIR) .lastbuild
	@echo $(PLATFORM) > .lastbuild.tmp
	@diff .lastbuild.tmp .lastbuild > /dev/null 2>&1 || $(MAKE) all-force

all-force: $(PLATFORM_DIR) FORCE
	cd $(GC_DIRNAME); \
	./configure --prefix='$(CURDIR)/$(PLATFORM)' --disable-threads; \
	$(MAKE) clean; \
	$(MAKE); \
	$(MAKE) check; \
	$(MAKE) install
	cp .lastbuild.tmp .lastbuild

clean:
	cd $(GC_DIRNAME) && $(MAKE) clean

clobber:
	-cd $(GC_DIRNAME) && $(MAKE) clean
	rm -rf ./$(PLATFORM) .lastbuild .lastbuild.tmp

.lastbuild:
	@echo "never-build" > .lastbuild

FORCE:
