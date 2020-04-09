#!/bin/bash
#standard data set is 1024 but should use 64 or 128
nl=8
N=32

echo nl=$nl
echo N=$N

#compile lu
chpl --fast lu.chpl -o lu

echo 'Cyclic (C)'
./lu -nl $nl --dist=C --N=$N --messages --timeit

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 4'
./lu -nl $nl --dist=CM --N=$N --correct --messages --timeit --minimumForAggregation=4

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 16'
./lu -nl $nl --dist=CM --N=$N --correct --messages --timeit --minimumForAggregation=16

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 100'
./lu -nl $nl --dist=CM --N=$N --correct --messages --timeit --minimumForAggregation=100

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 1000000'
./lu -nl $nl --dist=CM --N=$N --correct --messages --timeit --minimumForAggregation=1000000

echo 'Block (B)'
./lu -nl $nl --dist=B --N=$N --messages --timeit

echo 'No distribution (NONE)'
./lu -nl $nl --dist=NONE --N=$N --timeit
