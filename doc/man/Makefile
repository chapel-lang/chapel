PROGRAM = chpl

VERSION = $(shell ./grabVersion.pl)

TARGETS = $(PROGRAM).1 $(PROGRAM).pdf $(PROGRAM).html

all: $(TARGETS)

clean:
	rm *.1 *.ps *.pdf *.html

%.1: %.txt Makefile
	txt2man -s 1 -t $* -r $(VERSION) $< > $@

%.html: %.1
	rman -f HTML $< > $@

%.ps: %.1
	groff -man $< > $@

%.pdf: %.ps
	ps2pdf $< > $@
