all: gasnet

gasnet: FORCE
	-cd gasnet && echo "n" | ./Bootstrap
	cd gasnet && ./configure --prefix=$(CHPL_HOME)/third-party/gasnet/install --enable-segment-everything --enable-debug
	cd gasnet && $(MAKE) all
	cd gasnet && $(MAKE) install

FORCE:
