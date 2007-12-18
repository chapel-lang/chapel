all:
	collect_syntax.pl
	pdflatex -file-line-error -halt-on-error spec
	makeindex spec
	pdflatex -file-line-error -halt-on-error spec
	collect_syntax.pl # again to show errors

usedvi:
	collect_syntax.pl
	latex spec
	makeindex spec
	latex spec
	dvips -P cmz -t letter -o spec.ps spec.dvi
	ps2pdf spec.ps spec.pdf
	collect_syntax.pl # again to show errors

clean:
	rm -f *.aux *.blg *.bbl *.dvi *.log

clobber: clean
	rm -f spec.ps spec.pdf

#
# BLC: This is a somewhat silly rule, but on a clean build,
# I seem to need to run pdflatex in order to get an initial
# spec.idx file and make the rest of the Makefile
# work.  There may be a smarter way to do this.
#
#spec.idx:
#	-pdflatex -interaction batchmode spec_pl
