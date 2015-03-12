ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/..
endif

PROGRAM = chpl
CHPLDOC = chpldoc
VERSION = $(shell $(CHPL_MAKE_HOME)/util/devel/compVersion)

MANPAGE = man1/$(PROGRAM).1 man1/$(CHPLDOC).1
TARGETS = $(MANPAGE) $(PROGRAM).pdf $(CHPLDOC).pdf

TXT2MAN = $(CHPL_MAKE_HOME)/third-party/txt2man/txt2man

STARS = \*\*\*\*\*

default: echoman $(MANPAGE)

echoman: FORCE
	@echo $(STARS) ./ $(STARS)

all: $(TARGETS)

clean:
	rm $(TARGETS)

man1/%.1: %.txt Makefile
	mkdir -p $(shell dirname $@)
	$(TXT2MAN) -s 1 -I locale -I pthreads -I "on blocks" -I"Compiler Information Options" -I"Compilation Trace Options" -I"Module Processing Options" -I"Parallelism Control Options" -I"Optimization Control Options" -I"Run-time Semantic Check Options" -I "C Code Generation Options" -I"C Code Compilation Options" -I "LLVM Code Generation Options" -B"h" -B"c" -B"o" -B"cpp" -B"--no-cpp-lines" -I "Miscellaneous Options" -I "Documentation Options" -t $* -r $(VERSION) $< > $@

%.ps: man1/%.1
	groff -man $< > $@

%.pdf: %.ps
	ps2pdf $< $@

FORCE:
