PROGRAM = chpl

VERSION = $(shell ./grabVersion.pl)

TARGETS = $(PROGRAM).1 $(PROGRAM).pdf $(PROGRAM).html

all: $(TARGETS)

clean:
	rm *.1 *.ps *.pdf *.html

%.1: %.txt Makefile
	txt2man -s 1 -I locale -I pthreads -I"Compiler Information Options" -I"Compilation Trace Options" -I"Code Size Options" -I"Optimization Control Options" -I "Code Generation Options" -I"Linker Control Options" -B"cpp" -B"--no-cg-cpp-lines" -I "Miscellaneous Options" -t $* -r $(VERSION) $< > $@

%.html: %.1
	rman -f HTML $< > $@

%.ps: %.1
	groff -man $< > $@

%.pdf: %.ps
	ps2pdf $< > $@
