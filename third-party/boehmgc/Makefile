CHAPEL_ROOT = ../..
include $(CHAPEL_ROOT)/make/Makefile.base


all:
	mkdir -p ./$(PLATFORM)
	cd $(GC_DIRNAME); \
	./configure --prefix='$(CURDIR)/$(PLATFORM)' --disable-threads; \
	make; \
	make check; \
	make install

clean:
	cd $(GC_DIRNAME) && make clean
	rm -rf ./$(PLATFORM)/*

