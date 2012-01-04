CHAPEL_ROOT = ..

PROGRAM = chpl
VERSION = $(shell $(CHAPEL_ROOT)/util/devel/compVersion)

MANPAGE = man1/$(PROGRAM).1
TARGETS = $(MANPAGE) $(PROGRAM).pdf

TXT2MAN = $(THIRD_PARTY_DIR)/txt2man/txt2man

STARS = \*\*\*\*\*

default: echoman $(MANPAGE)

echoman: FORCE
	@echo $(STARS) ./ $(STARS)

all: $(TARGETS)

clean:
	rm $(TARGETS)

man1/%.1: %.txt Makefile
	$(TXT2MAN) -s 1 -I locale -I pthreads -I "on blocks" -I"Compiler Information Options" -I"Compilation Trace Options" -I"Module Processing Options" -I"Parallelism Control Options" -I"Optimization Control Options" -I"Run-time Semantic Check Options" -I "C Code Generation Options" -I"C Code Compilation Options" -B"h" -B"c" -B"o" -B"cpp" -B"--no-cpp-lines" -I "Miscellaneous Options" -t $* -r $(VERSION) $< > $@

%.ps: man1/%.1
	groff -man $< > $@

%.pdf: %.ps
	ps2pdf $< $@

FORCE:
