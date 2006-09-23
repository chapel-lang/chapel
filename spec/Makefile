all:
	perl preprocess.pl < spec.tex > spec_pl.tex
	latex spec_pl
	makeindex spec_pl
	cp spec_pl.idx spec.idx
	cp spec_pl.ind spec.ind
	latex spec_pl
	mv spec_pl.dvi spec.dvi
	dvips -P cmz -t letter -o spec.ps spec.dvi
	ps2pdf spec.ps spec.pdf
	cat .preprocess.pl.log

clean:
	rm -f *.aux *.blg *.bbl *.dvi *.log

clobber:
	rm -f spec.ps spec.pdf

#
# BLC: This is a somewhat silly rule, but on a clean build,
# I seem to need to run pdflatex in order to get an initial
# spec.idx file and make the rest of the Makefile
# work.  There may be a smarter way to do this.
#
#spec.idx:
#	-pdflatex -interaction batchmode spec_pl
