all:
	touch spec.ind
	./collect_syntax.pl
	pdflatex -file-line-error -halt-on-error spec
	makeindex spec
	pdflatex -file-line-error -halt-on-error spec
	./collect_syntax.pl # again to show errors

html:
	xhlatex spec "xhtml,uni-html4" " -cunihtf"

rst:
	#xhlatex spec "xhtml,uni-html4" " -cunihtf"
	#pandoc spec.html  -t rst -o spec.rst
	pandoc spec_pandoc.tex -t rst -o spec_pandoc.rst
	cat spec_pandoc.rst | perl fix.pl > spec_pandoc_fix.rst
	cp spec_pandoc_fix.rst ../doc/rst/technotes/spec.rst

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
