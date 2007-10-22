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

test:
	@$(MAKE) CHPL="$(CHPL)" CHPL_FLAGS="$(TESTFLAGS)" all > /dev/null 2>&1
	./hpl_test
	cat $(OUTPUTFILE)

clean: 
	rm -f $(TARGETS)
