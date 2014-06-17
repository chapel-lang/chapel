GENC = gen_c

CHPL = chpl
CHPL_FLAGS = 
OUTPUTFILE = HPL.out

SOURCES = HPL.chpl \
	Init.chpl \
	FactorSolve.chpl

TARGET = hpl_test

all: $(TARGET)

hpl_test: $(SOURCES)
	$(CHPL) $(CHPL_FLAGS) -o $@ $+

.PHONY: ps
PSDIR = ps
ps:
	@mkdir $(PSDIR)
	@for file in *.chpl; do out=$$(echo $$file|cut -d. -f1);    \
	    vim -e -c "set printoptions=paper:letter"               \
	    -c ":hardcopy > $(PSDIR)/$$out.ps" -c ":q" $$file;      \
	done
	@echo Postscript output is in $(PSDIR)
	@echo To 2-up any of the .ps files, do: "psnup -2 -pletter in.ps out.ps"

clean: 
	rm -f $(TARGET)
