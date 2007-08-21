GENC   = gen_c

CC     = chpl
CFLAGS = -g --savec=$(GENC) --short-names --print-passes
#CFLAGS = -g --savec=$(GENC) --short-names --print-passes --ccflags "-Wall"
#CFLAGS = -g --savec=$(GENC) --no-cg-cpp-lines --short-names --print-passes --ccflags "-Wall"

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
	$(CC) $(CFLAGS) -o $@ $+


.PHONY: clean
        
clean:
	rm -f 1d
	rm -rf $(GENC)
