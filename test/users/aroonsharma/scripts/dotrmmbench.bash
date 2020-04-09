#!/bin/bash
#standard data set is 1024 but should run on 128
nl=8
Dim=128

echo nl=$nl
echo Dim=$Dim

#compile trmm
chpl --fast trmm.chpl -o trmm

echo 'Cyclic (C)'
./trmm -nl $nl --dist=C --Dim=$Dim --messages --timeit 

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 4'
./trmm -nl $nl --dist=CM --Dim=$Dim --correct --messages --timeit --minimumForAggregation=4

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 16'
./trmm -nl $nl --dist=CM --Dim=$Dim --correct --messages --timeit --minimumForAggregation=16

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 100'
./trmm -nl $nl --dist=CM --Dim=$Dim --correct --messages --timeit --minimumForAggregation=100

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 1000000'
./trmm -nl $nl --dist=CM --Dim=$Dim --correct --messages --timeit --minimumForAggregation=1000000

echo 'Block (B)'
./trmm -nl $nl --dist=B --Dim=$Dim --messages --timeit

echo 'No distribution (NONE)'
./trmm -nl $nl --dist=NONE --Dim=$Dim --timeit
