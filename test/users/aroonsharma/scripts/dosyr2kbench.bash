#!/bin/bash
#standard sata set is 1024 but should run on 32 or 64 or 128
nl=8
M=256
N=256

echo nl=$nl
echo N=$N
echo M=$M

#compile syr2k
chpl --fast syr2k.chpl -o syr2k

echo 'Cyclic (C)'
./syr2k -nl $nl --dist=C --M=$M --N=$N --messages --timeit

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 4'
./syr2k -nl $nl --dist=CM --M=$M --N=$N --correct --messages --timeit --minimumForAggregation=4

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 16'
./syr2k -nl $nl --dist=CM --M=$M --N=$N --correct --messages --timeit --minimumForAggregation=16

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 100'
./syr2k -nl $nl --dist=CM --M=$M --N=$N --correct --messages --timeit --minimumForAggregation=100

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 1000000'
./syr2k -nl $nl --dist=CM --M=$M --N=$N --correct --messages --timeit --minimumForAggregation=1000000

echo 'Block (B)'
./syr2k -nl $nl --dist=B --M=$M --N=$N --messages --timeit

echo 'No distribution (NONE)'
./syr2k -nl $nl --dist=NONE --M=$M --N=$N --timeit