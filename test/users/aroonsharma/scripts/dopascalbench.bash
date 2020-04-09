#!/bin/bash
nl=8
blocksize=16

echo nl=$nl
echo n1=$n1
echo n2=$n2
echo blocksize=$blocksize

#compile pascal
chpl --fast pascal.chpl -o pascal

echo 'Block (B)'
./pascal -nl $nl --dist=B --n1=$n1 --n2=$n2 --messages
./pascal -nl $nl --dist=B --n1=$n1 --n2=$n2 --timeit

echo 'Cyclic (C)'
./pascal -nl $nl --dist=C --n1=$n1 --n2=$n2 --messages
./pascal -nl $nl --dist=C --n1=$n1 --n2=$n2 --timeit

echo 'Cyclic with modulo unrolling (CM) aggregation minimum=4'
./pascal -nl $nl --dist=CM --n1=$n1 --n2=$n2 --correct --minimumForAggregation=4
./pascal -nl $nl --dist=CM --n1=$n1 --n2=$n2 --messages --minimumForAggregation=4
./pascal -nl $nl --dist=CM --n1=$n1 --n2=$n2 --volume --minimumForAggregation=4
./pascal -nl $nl --dist=CM --n1=$n1 --n2=$n2 --timeit --minimumForAggregation=4

echo 'Cyclic with modulo unrolling (CM) aggregation minimum=16'
./pascal -nl $nl --dist=CM --n1=$n1 --n2=$n2 --correct --minimumForAggregation=16
./pascal -nl $nl --dist=CM --n1=$n1 --n2=$n2 --messages --minimumForAggregation=16
./pascal -nl $nl --dist=CM --n1=$n1 --n2=$n2 --volume --minimumForAggregation=16
./pascal -nl $nl --dist=CM --n1=$n1 --n2=$n2 --timeit --minimumForAggregation=16

echo 'Cyclic with modulo unrolling (CM) aggregation minimum=100'
./pascal -nl $nl --dist=CM --n1=$n1 --n2=$n2 --correct --minimumForAggregation=100
./pascal -nl $nl --dist=CM --n1=$n1 --n2=$n2 --messages --minimumForAggregation=100
./pascal -nl $nl --dist=CM --n1=$n1 --n2=$n2 --volume --minimumForAggregation=100
./pascal -nl $nl --dist=CM --n1=$n1 --n2=$n2 --timeit --minimumForAggregation=100

echo 'Cyclic with modulo unrolling (CM) aggregation minimum=1000000'
./pascal -nl $nl --dist=CM --n1=$n1 --n2=$n2 --correct --minimumForAggregation=1000000
./pascal -nl $nl --dist=CM --n1=$n1 --n2=$n2 --messages --minimumForAggregation=1000000
./pascal -nl $nl --dist=CM --n1=$n1 --n2=$n2 --volume --minimumForAggregation=1000000
./pascal -nl $nl --dist=CM --n1=$n1 --n2=$n2 --timeit --minimumForAggregation=1000000

echo 'Block Cyclic (BC) (pascal only)'
./pascal -nl $nl --dist=BC --n1=$n1 --n2=$n2 --messages --blocksize=$blocksize
./pascal -nl $nl --dist=BC --n1=$n1 --n2=$n2 --blocksize=$blocksize --timeit

echo 'Block Cyclic with modulo unrolling (BCM) (pascal only)'
./pascal -nl $nl --dist=BCM --n1=$n1 --n2=$n2 --correct --blocksize=$blocksize
./pascal -nl $nl --dist=BCM --n1=$n1 --n2=$n2 --messages --blocksize=$blocksize
./pascal -nl $nl --dist=BCM --n1=$n1 --n2=$n2 --volume --blocksize=$blocksize
./pascal -nl $nl --dist=BCM --n1=$n1 --n2=$n2 --blocksize=$blocksize --timeit

echo 'No distribution (NONE)'
./pascal -nl $nl --dist=NONE --n1=$n1 --n2=$n2 --timeit
