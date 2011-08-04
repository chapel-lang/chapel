#!/bin/bash
if ! type rman 2>&1 ; then
	echo "Please install rman"
	exit -1
fi
for F in man3/*.3 ; do
	rman -f html -r '%s.html' -S $F > ${F%.3}.html || (echo $F && break)
done
