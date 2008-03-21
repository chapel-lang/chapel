CHAPEL_ROOT = ..

PROGRAM = chpl
VERSION = $(shell $(CHAPEL_ROOT)/util/compVersion.pl)

MANPAGE = man1/$(PROGRAM).1
TARGETS = $(MANPAGE) $(PROGRAM).pdf $(PROGRAM).html

TXT2MAN = $(CHAPEL_ROOT)/third-party/txt2man/txt2man

STARS = \*\*\*\*\*

default: echoman $(MANPAGE)

echoman: FORCE
	@echo $(STARS) ./ $(STARS)

all: $(TARGETS)

clean:
	rm $(TARGETS)

man1/%.1: %.txt Makefile
	$(TXT2MAN) -s 1 -I locale -I pthreads -I "on blocks" -I"Compiler Information Options" -I"Compilation Trace Options" -I"Code Size Options" -I"Parallelism Control Options" -I"Optimization Control Options" -I"Run-time Semantic Check Options" -I "C Code Generation Options" -I"C Code Compilation Options" -B"cpp" -B"--no-cg-cpp-lines" -I "Miscellaneous Options" -t $* -r $(VERSION) $< > $@

%.html: %.1
	rman -f HTML $< > $@

%.ps: %.1
	groff -man $< > $@

%.pdf: %.ps
	ps2pdf $< > $@

FORCE:
