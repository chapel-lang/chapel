ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/..
endif

ifndef CHPL_MAKE_PYTHON
export CHPL_MAKE_PYTHON := $(shell $(CHPL_MAKE_HOME)/util/config/find-python.sh)
endif

PROGRAM = chpl
CHPLDOC = chpldoc

MANPAGE = man1/$(PROGRAM).1
CHPLDOC_MANPAGE = man1/$(CHPLDOC).1
TARGETS = $(MANPAGE) $(PROGRAM).pdf $(CHPLDOC).pdf

CHPLDEPS = $(shell $(CHPL_MAKE_PYTHON) $(CHPL_MAKE_HOME)/util/chplenv/chpl_home_utils.py --chpldeps)

RST2MAN = $(shell which rst2man.py || echo $(CHPL_MAKE_PYTHON) $(CHPLDEPS) rst2man.py)

STARS = \*\*\*\*\*

default: echoman $(MANPAGE)

echoman: FORCE
	@echo $(STARS) ./ $(STARS)

all: $(TARGETS)

chpldoc: $(CHPLDOC_MANPAGE)

clean:
	rm -f $(TARGETS) $(CHPLDOC_MANPAGE)

man1/%.1: %.rst Makefile
	mkdir -p $(shell dirname $@)
	sed "/conf$</r conf$<" $< > $<.tmp
	$(RST2MAN) $<.tmp > $<.tmp.out
	mv $<.tmp.out $@
	rm -f $<.tmp

%.ps: man1/%.1
	groff -man $< > $@

%.html: man1/%.1
	groff -T html -man $< > $@

%.pdf: %.ps
	ps2pdf $< $@

FORCE:
