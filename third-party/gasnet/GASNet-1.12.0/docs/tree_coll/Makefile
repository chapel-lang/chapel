
all: gasnet_teams.pdf designdoc.pdf

%.pdf: %.tex force
	pdflatex $<
	pdflatex $<

%.ps: %.tex force
	latex $<
	latex $<
	dvips $(@:.ps=.dvi) -o $@

clean:
	rm -f *.dvi *.aux main.out main.ps *~ *.log *.lot *.lof *.toc *.blg *.bbl

force:

.PHONY: force clean

