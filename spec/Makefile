all:
	touch spec.ind
	./collect_syntax.pl
	pdflatex -file-line-error -halt-on-error spec
	makeindex spec
	pdflatex -file-line-error -halt-on-error spec
	./collect_syntax.pl # again to show errors

usedvi:
	touch spec.ind
	./collect_syntax.pl
	latex spec
	makeindex spec
	latex spec
	dvips -P cmz -t letter -o spec.ps spec.dvi
	ps2pdf spec.ps spec.pdf
	./collect_syntax.pl # again to show errors

publish:
	cp spec.pdf ../doc/rst/language/chapelLanguageSpec.pdf

clean:
	rm -f *.aux *.blg *.bbl *.dvi *.log

clobber: clean
	rm -f spec.ps spec.pdf spec.ilg spec.out spec.toc spec.idx spec.ind
