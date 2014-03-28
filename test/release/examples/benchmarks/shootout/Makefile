CHPL = chpl

CHPL_FLAGS = --fast

TARGETS = \
	binarytrees \
	chameneosredux \
	fannkuch-redux \
	mandelbrot \
	nbody \
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

fannkuch-redux: fannkuch-redux.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

mandelbrot: mandelbrot.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

nbody: nbody.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) --no-warnings $<

spectralnorm: spectralnorm.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

threadring: threadring.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

FORCE:
