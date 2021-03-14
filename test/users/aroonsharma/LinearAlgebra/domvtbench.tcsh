#!/bin/tcsh
set nl=10
set Dim=4000

echo nl=$nl
echo Dim=$Dim

#compile mvt
chpl --fast mvt.chpl -o mvt

echo 'Cyclic (C)'
./mvt -nl $nl --dist=C --Dim=$Dim --messages
./mvt -nl $nl --dist=C --Dim=$Dim --timeit

echo 'Cyclic with modulo unrolling (CM)'
./mvt -nl $nl --dist=CM --Dim=$Dim --correct
./mvt -nl $nl --dist=CM --Dim=$Dim --messages
./mvt -nl $nl --dist=CM --Dim=$Dim --timeit

echo 'Block (B)'
./mvt -nl $nl --dist=B --Dim=$Dim --messages
./mvt -nl $nl --dist=B --Dim=$Dim --timeit

echo 'No distribution (NONE)'
./mvt -nl $nl --dist=NONE --Dim=$Dim --timeit
