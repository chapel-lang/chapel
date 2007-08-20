CC     = chpl
CFLAGS =

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
	$(CC) -o $@ $+


.PHONY: clean
        
clean:
	rm -f 1d
