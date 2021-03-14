#!/bin/bash
nl=8
N=512
M=512

echo nl=$nl
echo N=$N
echo M=$M

#compile covariance
chpl --fast covariance.chpl -o covariance

echo 'Cyclic (C)'
./covariance -nl $nl --dist=C --N=$N --M=$M --messages --timeit

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 4'
./covariance -nl $nl --dist=CM --N=$N --M=$M --correct --messages --timeit --minimumForAggregation=4

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 16'
./covariance -nl $nl --dist=CM --N=$N --M=$M --correct --messages --timeit --minimumForAggregation=16

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 100'
./covariance -nl $nl --dist=CM --N=$N --M=$M --correct --messages --timeit --minimumForAggregation=100

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 1000000'
./covariance -nl $nl --dist=CM --N=$N --M=$M --correct --messages --timeit --minimumForAggregation=1000000

echo 'Block (B)'
./covariance -nl $nl --dist=B --N=$N --M=$M --messages --timeit

echo 'No distribution (NONE)'
./covariance -nl $nl --dist=NONE --N=$N --M=$M --timeit
