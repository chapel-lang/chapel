all:
	latex reference.tex
	dvips -t landscape -o reference.ps reference.dvi
	ps2pdf reference.ps

clean:
	rm -f *.aux *.blg *.bbl *.dvi *.log

clobber:
	rm -f reference.ps reference.pdf

