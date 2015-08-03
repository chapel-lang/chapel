#!/bin/tcsh
set nl=10
set N=128

echo nl=$nl
echo N=$N

#compile cholesky
chpl --fast cholesky.chpl -o cholesky

echo 'Cyclic (C)'
./cholesky -nl $nl --dist=C --N=$N --messages
./cholesky -nl $nl --dist=C --N=$N --timeit

echo 'Cyclic with modulo unrolling (CM)'
./cholesky -nl $nl --dist=CM --N=$N --correct
./cholesky -nl $nl --dist=CM --N=$N --messages
./cholesky -nl $nl --dist=CM --N=$N --timeit

echo 'Block (B)'
./cholesky -nl $nl --dist=B --N=$N --messages
./cholesky -nl $nl --dist=B --N=$N --timeit

echo 'No distribution (NONE)'
./cholesky -nl $nl --dist=NONE --N=$N --timeit