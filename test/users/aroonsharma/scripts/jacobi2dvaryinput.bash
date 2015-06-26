#!/bin/bash
nl=8
epsilon=0.05

echo "jacobi2d vary input"
echo nl=$nl
echo n=$n
echo epsilon=$epsilon

#compile jacobi-2d
chpl jacobi-2d.chpl --fast -o jacobi-2d

for n in {101..200..1}
	do
		echo "Cyclic (C) n=$n"
		./jacobi-2d -nl $nl --dist=C --n=$n --messages --timeit

		echo "Cyclic with modulo unrolling (CM) n=$n"
		./jacobi-2d -nl $nl --dist=CM --n=$n --messages --timeit
	done