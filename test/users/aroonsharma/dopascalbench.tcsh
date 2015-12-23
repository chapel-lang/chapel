#!/bin/tcsh
set nl=10
set n1=100000
set n2=100003
set blocksize=16

echo nl=$nl
echo n1=$n1
echo n2=$n2
echo blocksize=$blocksize


if -e pascal then
else
  chpl --fast pascal.chpl -o pascal
endif

echo 'Block (B)'
./pascal -nl $nl --dist=B --n1=$n1 --n2=$n2 --messages
./pascal -nl $nl --dist=B --n1=$n1 --n2=$n2 --timeit

echo 'Cyclic (C)'
./pascal -nl $nl --dist=C --n1=$n1 --n2=$n2 --messages
./pascal -nl $nl --dist=C --n1=$n1 --n2=$n2 --timeit

echo 'Cyclic with modulo unrolling (CM)'
./pascal -nl $nl --dist=CM --n1=$n1 --n2=$n2 --correct
./pascal -nl $nl --dist=CM --n1=$n1 --n2=$n2 --messages
./pascal -nl $nl --dist=CM --n1=$n1 --n2=$n2 --volume
./pascal -nl $nl --dist=CM --n1=$n1 --n2=$n2 --timeit

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
