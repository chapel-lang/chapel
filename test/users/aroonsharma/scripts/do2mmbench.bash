#!/bin/bash
nl=8
size=128

echo nl=$nl
echo size=$size

#compile 2mm
chpl --fast 2mm.chpl -o 2mm

echo 'Cyclic (C)'
./2mm -nl $nl --dist=C --size=$size --messages --timeit

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 4'
./2mm -nl $nl --dist=CM --size=$size --correct --messages --timeit --minimumForAggregation=4

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 16'
./2mm -nl $nl --dist=CM --size=$size --correct --messages --timeit --minimumForAggregation=16

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 100'
./2mm -nl $nl --dist=CM --size=$size --correct --messages --timeit --minimumForAggregation=100

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 1000000'
./2mm -nl $nl --dist=CM --size=$size --correct --messages --timeit --minimumForAggregation=1000000

echo 'Block (B)'
./2mm -nl $nl --dist=B --size=$size --messages --timeit

echo 'No distribution (NONE)'
./2mm -nl $nl --dist=NONE --size=$size --timeit
