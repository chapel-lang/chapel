#!/bin/tcsh
set nl=10
set M=256
set N=256
set P=256

echo nl=$nl
echo M=$M
echo N=$N
echo P=$P

#compile fdtd-apml
chpl --fast fdtd-apml.chpl -o fdtd-apml

echo 'Cyclic (C)'
./fdtd-apml -nl $nl --dist=C --M=$M --N=$N --P=$P --messages
./fdtd-apml -nl $nl --dist=C --M=$M --N=$N --P=$P --timeit

echo 'Cyclic with modulo unrolling (CM)'
./fdtd-apml -nl $nl --dist=CM --M=$M --N=$N --P=$P --correct
./fdtd-apml -nl $nl --dist=CM --M=$M --N=$N --P=$P --messages
./fdtd-apml -nl $nl --dist=CM --M=$M --N=$N --P=$P --timeit

echo 'Block (B)'
./fdtd-apml -nl $nl --dist=B --M=$M --N=$N --P=$P --messages
./fdtd-apml -nl $nl --dist=B --M=$M --N=$N --P=$P --timeit

echo 'No distribution (NONE)'
./fdtd-apml -nl $nl --dist=NONE --M=$M --N=$N --P=$P --timeit
