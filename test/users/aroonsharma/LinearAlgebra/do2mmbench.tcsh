#!/bin/tcsh
set nl=10
set size=128

echo nl=$nl
echo size=$size

#compile 2mm
chpl --fast 2mm.chpl -o 2mm

echo 'Cyclic (C)'
./2mm -nl $nl --dist=C --size=$size --messages
./2mm -nl $nl --dist=C --size=$size --timeit

echo 'Cyclic with modulo unrolling (CM)'
./2mm -nl $nl --dist=CM --size=$size --correct
./2mm -nl $nl --dist=CM --size=$size --messages
./2mm -nl $nl --dist=CM --size=$size --timeit

echo 'Block (B)'
./2mm -nl $nl --dist=B --size=$size --messages
./2mm -nl $nl --dist=B --size=$size --timeit

echo 'No distribution (NONE)'
./2mm -nl $nl --dist=NONE --size=$size --timeit
