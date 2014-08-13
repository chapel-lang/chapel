.PHONY : all

all : compile run

compile :
	chpl chunker.chpl --fast
	
run :
	time ./a.out < ~/fasta.big > /dev/null
