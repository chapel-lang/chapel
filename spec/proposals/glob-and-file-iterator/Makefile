PAPER=glob_proposal
TR=tr
LATEX=pdflatex
EPS=epstopdf
UNAME := $(shell uname)

ALLFIGS :=
#graphs/microbench.pdf graphs/microbench_many.pdf graphs/Ivars_Syncvars.pdf \
#   graphs/multiple.pdf graphs/multiple.pdf graphs/parfib_with_ivars.pdf \
#   graphs/parfib_without_ivars.pdf graphs/qthread_syncvars.pdf graphs/wavefront.pdf

.SUFFIXES: .eps .pdf

default: quick

all: ed

quick:
	${LATEX} ${PAPER}.tex
	${MAKE} open

# Build the final PDF:	
paper: figs
	${LATEX} ${PAPER}.tex
	bibtex ${PAPER}
	${LATEX} ${PAPER}.tex
	bibtex ${PAPER}
	${LATEX} ${PAPER}.tex
	${MAKE} open

figs: ${ALLFIGS}

# define a suffix rule for .eps -> .pdf
.eps.pdf :
	${EPS} $<

# Build with editorial markup rather than the final pdf:
ed: editingmarks
editingmarks:
	touch activateeditingmarks
#	${MAKE} paper
	${MAKE} quick

plain: 
	rm activateeditingmarks
	${MAKE} paper

# A little shorthand for Ryan:
grey:
	touch activategreybg.tex
	${MAKE} paper
white:
	rm -f activategreybg.tex
	${MAKE} paper


# Open the PDF in a viewer after building:
open: ${PAPER}.pdf
ifeq (${UNAME}, Darwin)
	open ${PAPER}.pdf
endif
ifeq (${UNAME}, Linux)
	evince ${PAPER}.pdf &
endif

clean: 
	rm -f *.out *.aux *.bbl *.log *.blg


