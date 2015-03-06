CHPL = chpl

CHPL_FLAGS = --fast

TARGETS = \
	binarytrees \
	chameneosredux \
	fannkuchredux \
	mandelbrot \
	meteor \
	meteor-fast \
	nbody \
	pidigits \
	spectralnorm \
	threadring

REALS = $(TARGETS:%=%_real)

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)


binarytrees: binarytrees.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

chameneosredux: chameneosredux.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

fannkuchredux: fannkuchredux.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

mandelbrot: mandelbrot.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

meteor: meteor.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

meteor-fast: meteor-fast.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

nbody: nbody.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) --no-warnings $<

pidigits: pidigits.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

spectralnorm: spectralnorm.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

threadring: threadring.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

FORCE:
