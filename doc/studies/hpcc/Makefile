all:
	latex submitsc06
	latex submitsc06
	dvips -P cmz -t letter -o submitsc06.ps submitsc06.dvi
	ps2pdf submitsc06.ps submitsc06.pdf

clean:
	rm -f *.aux *.blg *.bbl *.dvi *.log

clobber:
	rm -f submitsc06.ps submitsc06.pdf

