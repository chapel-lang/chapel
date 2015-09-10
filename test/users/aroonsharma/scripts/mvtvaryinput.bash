#!/bin/bash
nl=8

echo "mvt vary input"
echo nl=$nl

#compile mvt
chpl --fast mvt.chpl -o mvt

for n in {410..800..10}
  do
    echo "Cyclic (C) Dim=$n"
    ./mvt -nl $nl --dist=C --Dim=$n --messages --timeit

    echo "Cyclic with modulo unrolling (CM) Dim=$n"
    ./mvt -nl $nl --dist=CM --Dim=$n --messages --timeit
  done
