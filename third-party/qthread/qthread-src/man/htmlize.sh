#!/bin/bash
if ! type rman 2>&1 ; then
	echo "Please install rman"
	exit -1
fi
mkdir html
for F in man3/*.3 ; do
	newfile="${F%.3}.html"
	newfile="html/${newfile#man3/}"
	rman -f html -r '%s.html' -S "$F" > "${newfile}" || (echo "$F" && break)
done
