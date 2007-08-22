GENC   = gen_c

CC     = chpl
CFLAGS = --instantiate-max 512 -g --savec=$(GENC) --short-names --print-passes
#CFLAGS += --ccflags "-Wall"
#CFLAGS += --no-cg-cpp-lines

SOURCES= Fn1d.chpl       \
         FTree.chpl      \
         Function1d.chpl \
         HgCoeffs.chpl   \
         MadFn1d.chpl    \
         Quadrature.chpl \
         Tensor.chpl     \
         TwoScaleCoeffs.chpl

all: 1d


1d: $(SOURCES)
	$(CC) $(CFLAGS) --main-module Function1d -o $@ $+


.PHONY: distrib
DISTDIR = mad_chapel
distrib: clean
	mkdir $(DISTDIR)
	tar -X DIST_EXCLUDE --exclude=$(DISTDIR) -c * | tar -C $(DISTDIR) -xf -
	@echo "++ Distribution has been built in $(DISTDIR)"


.PHONY: clean
clean:
	rm -f 1d
	rm -rf $(GENC)
