#script to determine parallel chunk sizes for different locale runs of fdtd-2d
#!/bin/bash
nl=8

for n in {2..8..1}
	do		
		./fdtd-2d -nl $n --dist=CM --M=1000 --N=1000 --TMAX=50 > fdtd2dlocales.logc
		lines=0
		while read line
		do
			sum=$((sum+line))
			lines=$((lines+1))
		done < $1
		echo $((sum/lines))
	done