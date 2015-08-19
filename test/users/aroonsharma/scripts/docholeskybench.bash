#!/bin/bash
nl=8
N=256

echo nl=$nl
echo N=$N

#compile cholesky
chpl --fast cholesky.chpl -o cholesky

echo 'Cyclic (C)'
./cholesky -nl $nl --dist=C --N=$N --messages --timeit

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 4'
./cholesky -nl $nl --dist=CM --N=$N --correct --messages --timeit --minimumForAggregation=4

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 16'
./cholesky -nl $nl --dist=CM --N=$N --correct --messages --timeit --minimumForAggregation=16

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 100'
./cholesky -nl $nl --dist=CM --N=$N --correct --messages --timeit --minimumForAggregation=100

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 1000000'
./cholesky -nl $nl --dist=CM --N=$N --correct --messages --timeit --minimumForAggregation=1000000

echo 'Block (B)'
./cholesky -nl $nl --dist=B --N=$N --messages --timeit

echo 'No distribution (NONE)'
./cholesky -nl $nl --dist=NONE --N=$N --timeit
