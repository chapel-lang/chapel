#!/bin/bash
#standard data set is 4000 should be able to run on full set
nl=8
Dim=4000

echo nl=$nl
echo Dim=$Dim

#compile mvt
chpl --fast mvt.chpl -o mvt

echo 'Cyclic (C)'
./mvt -nl $nl --dist=C --Dim=$Dim --messages --timeit

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 4'
./mvt -nl $nl --dist=CM --Dim=$Dim --correct --messages --timeit --minimumForAggregation=4

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 16'
./mvt -nl $nl --dist=CM --Dim=$Dim --correct --messages --timeit --minimumForAggregation=16

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 100'
./mvt -nl $nl --dist=CM --Dim=$Dim --correct --messages --timeit --minimumForAggregation=100

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 1000000'
./mvt -nl $nl --dist=CM --Dim=$Dim --correct --messages --timeit --minimumForAggregation=1000000

echo 'Block (B)'
./mvt -nl $nl --dist=B --Dim=$Dim --messages --timeit

echo 'No distribution (NONE)'
./mvt -nl $nl --dist=NONE --Dim=$Dim --timeit
