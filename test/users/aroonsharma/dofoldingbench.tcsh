#!/bin/tcsh
set nl=10
set iterations=10
set n=50400

echo nl=$nl
echo n=$n
echo iterations=$iterations

#rm folding
if -e folding then
else
  chpl --fast folding.chpl -o folding
endif

echo 'Block (B)'
./folding -nl $nl --dist=B --n=$n --iterations=$iterations --messages
./folding -nl $nl --dist=B --n=$n --iterations=$iterations --timeit

echo 'Cyclic (C)'
./folding -nl $nl --dist=C --n=$n --iterations=$iterations --messages
./folding -nl $nl --dist=C --n=$n --iterations=$iterations --timeit

echo 'Cyclic with modulo unrolling (CM)'
./folding -nl $nl --dist=CM --n=$n --iterations=$iterations --correct
./folding -nl $nl --dist=CM --n=$n --iterations=$iterations --messages
./folding -nl $nl --dist=CM --n=$n --iterations=$iterations --timeit

echo 'No distribution (NONE)'
./folding -nl $nl --dist=NONE --n=$n --iterations=$iterations --timeit
