#!/bin/tcsh
set nl=10
set M=128
set N=128

echo nl=$nl
echo N=$N
echo M=$M

#compile syrk
chpl --fast syrk.chpl -o syrk

echo 'Cyclic (C)'
./syrk -nl $nl --dist=C --M=$M --N=$N --messages
./syrk -nl $nl --dist=C --M=$M --N=$N --timeit

echo 'Cyclic with modulo unrolling (CM)'
./syrk -nl $nl --dist=CM --M=$M --N=$N --correct
./syrk -nl $nl --dist=CM --M=$M --N=$N --messages
./syrk -nl $nl --dist=CM --M=$M --N=$N --timeit

echo 'Block (B)'
./syrk -nl $nl --dist=B --M=$M --N=$N --messages
./syrk -nl $nl --dist=B --M=$M --N=$N --timeit

echo 'No distribution (NONE)'
./syrk -nl $nl --dist=NONE --M=$M --N=$N --timeit
