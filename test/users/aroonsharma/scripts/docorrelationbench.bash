#!/bin/bash
nl=8
N=512
M=512

echo nl=$nl
echo N=$N
echo M=$M

#compile correlation
chpl --fast correlation.chpl -o correlation

echo 'Cyclic (C)'
./correlation -nl $nl --dist=C --N=$N --M=$M --messages --timeit

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 4'
./correlation -nl $nl --dist=CM --N=$N --M=$M --correct --messages --timeit --minimumForAggregation=4

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 16'
./correlation -nl $nl --dist=CM --N=$N --M=$M --correct --messages --timeit --minimumForAggregation=16

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 100'
./correlation -nl $nl --dist=CM --N=$N --M=$M --correct --messages --timeit --minimumForAggregation=100

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 1000000'
./correlation -nl $nl --dist=CM --N=$N --M=$M --correct --messages --timeit --minimumForAggregation=1000000

echo 'Block (B)'
./correlation -nl $nl --dist=B --N=$N --M=$M --messages --timeit

echo 'No distribution (NONE)'
./correlation -nl $nl --dist=NONE --N=$N --M=$M --timeit
