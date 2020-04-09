#!/bin/tcsh
set nl=10
set epsilon=0.05
set n=400

echo nl=$nl
echo n=$n
echo epsilon=$epsilon

#rm stencil9
if -e stencil9 then
else
  chpl --fast stencil9.chpl -o stencil9
endif

echo 'Block (B)'
./stencil9 -nl $nl --dist=B --n=$n --epsilon=$epsilon --messages
./stencil9 -nl $nl --dist=B --n=$n --epsilon=$epsilon --timeit

echo 'Cyclic (C)'
./stencil9 -nl $nl --dist=C --n=$n --epsilon=$epsilon --messages
./stencil9 -nl $nl --dist=C --n=$n --epsilon=$epsilon --timeit

echo 'Cyclic with modulo unrolling (CM)'
./stencil9 -nl $nl --dist=CM --n=$n --epsilon=$epsilon --correct
./stencil9 -nl $nl --dist=CM --n=$n --epsilon=$epsilon --messages
./stencil9 -nl $nl --dist=CM --n=$n --epsilon=$epsilon --timeit

echo 'No distribution (NONE)'
./stencil9 -nl $nl --dist=NONE --n=$n --timeit
