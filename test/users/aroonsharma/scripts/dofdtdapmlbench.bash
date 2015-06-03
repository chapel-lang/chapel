#!/bin/bash
#standard data set is 256 but should run on 32 or 64
nl=8
M=64
N=64
P=64

echo nl=$nl
echo M=$M
echo N=$N
echo P=$P

#compile fdtd-apml
chpl --fast fdtd-apml.chpl -o fdtd-apml

echo 'Cyclic (C)'
./fdtd-apml -nl $nl --dist=C --M=$M --N=$N --P=$P --messages --timeit

echo 'Cyclic with modulo unrolling (CM) aggregation limit = 4'
./fdtd-apml -nl $nl --dist=CM --M=$M --N=$N --P=$P --correct --messages --timeit --minimumForAggregation=4

echo 'Cyclic with modulo unrolling (CM) aggregation limit = 16'
./fdtd-apml -nl $nl --dist=CM --M=$M --N=$N --P=$P --correct --messages --timeit --minimumForAggregation=16

echo 'Cyclic with modulo unrolling (CM) aggregation limit = 100'
./fdtd-apml -nl $nl --dist=CM --M=$M --N=$N --P=$P --correct --messages --timeit --minimumForAggregation=100

echo 'Cyclic with modulo unrolling (CM) aggregation limit = 1000000'
./fdtd-apml -nl $nl --dist=CM --M=$M --N=$N --P=$P --correct --messages --timeit --minimumForAggregation=1000000

echo 'Block (B)'
./fdtd-apml -nl $nl --dist=B --M=$M --N=$N --P=$P --messages --timeit

echo 'No distribution (NONE)'
./fdtd-apml -nl $nl --dist=NONE --M=$M --N=$N --P=$P --timeit