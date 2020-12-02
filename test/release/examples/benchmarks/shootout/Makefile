CHPL = chpl

CHPL_FLAGS = --fast

TARGETS = \
	binarytrees \
	chameneosredux \
	chameneosredux-fast \
	fasta \
	fannkuchredux \
	knucleotide \
	mandelbrot \
	mandelbrot-fast \
	meteor \
	meteor-fast \
	nbody \
	pidigits \
	pidigits-fast \
	regexdna \
	regexdna-redux \
	revcomp \
	revcomp-fast \
	spectralnorm \
	threadring

CHPL_MAKE_REGEXP=$(shell python3 $(CHPL_HOME)/util/chplenv/chpl_regexp.py)
CHPL_MAKE_GMP=$(shell python3 $(CHPL_HOME)/util/chplenv/chpl_gmp.py)

REALS = $(TARGETS:%=%_real)

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)


binarytrees: binarytrees.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) $<

chameneosredux: chameneosredux.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) $<

chameneosredux-fast: chameneosredux-fast.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) $<

fasta: fasta.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) $<

fannkuchredux: fannkuchredux.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) $<

knucleotide: knucleotide.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) --no-warnings $<

mandelbrot: mandelbrot.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) $<

mandelbrot-fast: mandelbrot-fast.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) $<

meteor: meteor.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) $<

meteor-fast: meteor-fast.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) $<

nbody: nbody.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) --no-warnings $<

pidigits: pidigits.chpl
	@+if [ "$(CHPL_MAKE_GMP)" != "none" ]; then \
	$(CHPL) -o $@ $(CHPL_FLAGS) $<; \
	else \
	echo "";\
	echo "Can't build pidigits without CHPL_GMP being enabled. See for details:"; \
	echo "https://chapel-lang.org/docs/usingchapel/chplenv.html#chpl-gmp"; \
	echo ""; \
	fi

pidigits-fast: pidigits-fast.chpl
	@+if [ "$(CHPL_MAKE_GMP)" != "none" ]; then \
	$(CHPL) -o $@ $(CHPL_FLAGS) $<; \
	else \
	echo "";\
	echo "Can't build pidigits-fast without CHPL_GMP being enabled. See for details:"; \
	echo "https://chapel-lang.org/docs/usingchapel/chplenv.html#chpl-gmp"; \
	echo ""; \
	fi

regexdna: regexdna.chpl
	@+if [ "$(CHPL_REGEXP)" != "none" ]; then \
	$(CHPL) -o $@ $(CHPL_FLAGS) $<; \
	else \
	echo "";\
	echo "Can't build regexdna without CHPL_REGEXP being enabled. See for details:"; \
	echo "https://chapel-lang.org/docs/usingchapel/chplenv.html#chpl-regexp"; \
	echo ""; \
	fi

regexdna-redux: regexdna-redux.chpl
	@+if [ "$(CHPL_REGEXP)" != "none" ]; then \
	$(CHPL) -o $@ $(CHPL_FLAGS) $<; \
	else \
	echo "";\
	echo "Can't build regexdna-redux without CHPL_REGEXP being enabled. See for details:"; \
	echo "https://chapel-lang.org/docs/usingchapel/chplenv.html#chpl-regexp"; \
	echo ""; \
	fi

revcomp: revcomp.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) $<

revcomp-fast: revcomp-fast.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) $<

spectralnorm: spectralnorm.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) $<

threadring: threadring.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) $<

FORCE:
