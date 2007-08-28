GENC   = gen_c

CC     = chpl
CFLAGS = --instantiate-max 512 -g --savec=$(GENC) --short-names --print-passes
#CFLAGS += --ccflags "-Wall"
#CFLAGS += --no-cg-cpp-lines

SOURCES= Fn1d.chpl       \
         FTree.chpl      \
         Function1d.chpl \
         MadFn1d.chpl    \
         Quadrature.chpl \
         Tensor.chpl     \
         TwoScale.chpl

TARGETS= test_diff       \
         test_gaxpy

all: $(TARGETS)


test_diff: $(SOURCES) test_diff.chpl
	$(CC) $(CFLAGS) --main-module $@ -o $@ $+


test_gaxpy: $(SOURCES) test_gaxpy.chpl
	$(CC) $(CFLAGS) --main-module $@ -o $@ $+


.PHONY: distrib
DISTDIR = mad_chapel
distrib: clean
	mkdir $(DISTDIR)
	tar -X DIST_EXCLUDE --exclude=$(DISTDIR) -c * | tar -C $(DISTDIR) -xf -
	@echo "++ Distribution has been built in $(DISTDIR)"


.PHONY: clean
clean:
	rm -f $(TARGETS)
	rm -rf $(GENC)
