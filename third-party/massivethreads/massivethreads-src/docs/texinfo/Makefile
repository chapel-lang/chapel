#
# Makefile for generating documents
# 
# massivethreads.pdf
# massivethreads.info
# massivethreads.html
#
# distribution contains them so you normally have to run this.
#
# to generate them, you need
# (1) makeinfo (install texinfo package).
# (2) texinfo.tex in this directory (install texinfo package and then
#     copy texinfo.tex here).
# (3) dvipdfm (for gxpman.pdf)
#

manual = massivethreads

#
# svg files
# 
svgs     := $(wildcard svg/*.svg)
svg_epss := $(patsubst %.svg,%.eps,$(svgs))
svg_pdfs := $(patsubst %.svg,%.pdf,$(svgs))
svg_bbs  := $(patsubst %.svg,%.bb,$(svgs))
svg_pngs := $(patsubst %.svg,%.png,$(svgs))
#
# gnuplot files
#
gpls     := $(wildcard gpl/*.gpl)
gpl_pngs := $(patsubst %.gpl,%.png,$(gpls))
gpl_epss := $(patsubst %.gpl,%.eps,$(gpls))
# 
# images
# 
pngs         := $(filter-out %_resized.png,$(wildcard img/*.png))
resized_pngs := $(patsubst %.png,%_resized.png,$(pngs))
resized_epss := $(patsubst %.png,%_resized.eps,$(pngs))

images := $(svg_pngs) $(gpl_pngs) $(resized_epss) 

all : $(manual).pdf $(manual).info $(manual).html 

$(manual).tex : $(manual)_src.tex
	emacs -q -batch --load=make_texinfo.el

$(manual).info : $(manual).tex
	makeinfo --fill-column=70 $(manual).tex

$(manual).dvi : $(manual).tex $(images)
	tex $(manual).tex

$(manual).pdf : $(manual).dvi
	dvipdfm $(manual).dvi

$(manual).html : $(manual).tex $(images)
	makeinfo --no-split --force --html --css-include=massivethreads.css --fill-column=70 --output $@ $< 

#
# svg files
# 

$(svg_epss) : %.eps : %.svg
	inkscape --export-eps=$@ $<

$(svg_pdfs) : %.pdf : %.eps
	convert $< $@

$(svg_bbs) : %.bb : %.pdf
	ebb $< 

$(svg_pngs) : %.png : %.eps
	convert $< -resize 600x $@

#
# gnuplot files
#

$(gpl_epss) : %.eps : %.gpl
	echo -n | gnuplot -e 'set terminal postscript eps enhanced color' -e 'set output "$@"' $< 

$(gpl_pngs) : %.png : %.eps
	convert $< -resize 600x $@

# 
# images
# 

$(resized_pngs) : %_resized.png : %.png
	convert $< -resize 600x400 $@

$(resized_epss) : %_resized.eps : %_resized.png
	convert $< $@

clean :
	rm -f $(manual).aux $(manual).log $(manual).cp $(manual).info-1 $(manual).md 
	rm -f $(manual).toc $(manual).info-2 $(manual).pg $(manual).tp $(manual).fn $(manual).ky $(manual).vr $(manual).dvi
