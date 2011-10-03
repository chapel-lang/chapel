#!/bin/bash

unamestr=`uname`
if [[ "$unamestr" == 'Linux' ]]; then
   chplbin='./bin/linux64/chpl'
elif [[ "$unamestr" == 'Darwin' ]]; then
   chplbin='./bin/darwin/chpl'
fi

success=0
total=0
finalmsg="\n"

for file in concept_tests/jonathan/*.chpl
do
	total=$((total+1))
	# echo "------[${file}]------"
	output=`${chplbin} ${file} 2>&1`
	if [[ $output == *SUCCESS* ]]
        then
		echo -n "."		
		success=$((success+1))
	else
		if [[ $file == *mismatch* ]]
		then
			echo -n "."
			success=$((success+1))
		else
			msg="${file} failed: ${output}\n"
			finalmsg=$finalmsg$msg
			echo -n "E"
		fi
	fi
done
echo -e $finalmsg
echo "${success} out of ${total} passed."
