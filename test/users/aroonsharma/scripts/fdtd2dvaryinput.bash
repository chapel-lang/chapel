#!/bin/bash
nl=8
TMAX=50

echo "fdtd2d vary input"
echo nl=$nl
echo TMAX=$TMAX

#compile fdtd-2d
chpl --fast fdtd-2d.chpl -o fdtd-2d

for n in {10..200..5}
  do
    echo "Cyclic (C)"
    ./fdtd-2d -nl $nl --dist=C --M=$n --N=$n --TMAX=$TMAX --messages --timeit

    echo "Cyclic with modulo unrolling (CM)"
    ./fdtd-2d -nl $nl --dist=CM --M=$n --N=$n --TMAX=$TMAX --messages --timeit
  done
