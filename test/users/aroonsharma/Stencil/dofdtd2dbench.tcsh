#!/bin/tcsh
set nl=4
set M=8
set N=8

echo nl=$nl
echo M=$M
echo N=$N

#compile fdtd-2d
chpl --fast fdtd-2d.chpl -o fdtd-2d

echo 'Cyclic (C)'
./fdtd-2d -nl $nl --dist=C --M=$M --N=$N --messages
./fdtd-2d -nl $nl --dist=C --M=$M --N=$N --timeit

echo 'Cyclic with modulo unrolling (CM)'
./fdtd-2d -nl $nl --dist=CM --M=$M --N=$N --correct
./fdtd-2d -nl $nl --dist=CM --M=$M --N=$N --messages
./fdtd-2d -nl $nl --dist=CM --M=$M --N=$N --timeit

echo 'Block (B)'
./fdtd-2d -nl $nl --dist=B --M=$M --N=$N --messages
./fdtd-2d -nl $nl --dist=B --M=$M --N=$N --timeit

echo 'No distribution (NONE)'
./fdtd-2d -nl $nl --dist=NONE --M=$M --N=$N --timeit
