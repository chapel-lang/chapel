#!/bin/tcsh
set nl=10
set N=128
set M=128

echo nl=$nl
echo N=$N
echo M=$M

#compile correlation
chpl --fast correlation.chpl -o correlation

echo 'Cyclic (C)'
./correlation -nl $nl --dist=C --N=$N --M=$M --messages
./correlation -nl $nl --dist=C --N=$N --M=$M --timeit

echo 'Cyclic with modulo unrolling (CM)'
./correlation -nl $nl --dist=CM --N=$N --M=$M --correct
./correlation -nl $nl --dist=CM --N=$N --M=$M --messages
./correlation -nl $nl --dist=CM --N=$N --M=$M --timeit

echo 'Block (B)'
./correlation -nl $nl --dist=B --N=$N --M=$M --messages
./correlation -nl $nl --dist=B --N=$N --M=$M --timeit

echo 'No distribution (NONE)'
./correlation -nl $nl --dist=NONE --N=$N --M=$M --timeit
