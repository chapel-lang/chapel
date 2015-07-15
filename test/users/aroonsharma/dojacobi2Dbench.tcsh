#!/bin/tcsh
set nl=10
set epsilon=0.05
set n=400

echo nl=$nl
echo n=$n
echo epsilon=$epsilon

#rm jacobi
if -e jacobi then
else
	chpl --fast jacobi.chpl -o jacobi
endif

echo 'Block (B)'
./jacobi -nl $nl --dist=B --n=$n --epsilon=$epsilon --messages
./jacobi -nl $nl --dist=B --n=$n --epsilon=$epsilon --timeit

echo 'Cyclic (C)'
./jacobi -nl $nl --dist=C --n=$n --epsilon=$epsilon --messages
./jacobi -nl $nl --dist=C --n=$n --epsilon=$epsilon --timeit

echo 'Cyclic with modulo unrolling (CM)'
./jacobi -nl $nl --dist=CM --n=$n --epsilon=$epsilon --correct
./jacobi -nl $nl --dist=CM --n=$n --epsilon=$epsilon --messages
./jacobi -nl $nl --dist=CM --n=$n --epsilon=$epsilon --timeit

echo 'No distribution (NONE)'
./jacobi -nl $nl --dist=NONE --n=$n --timeit
