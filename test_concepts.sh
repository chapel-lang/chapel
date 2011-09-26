#!/bin/bash

success=0
total=0
finalmsg=""

for file in concept_tests/jonathan/*.chpl
do
	total=$((total+1))
	# echo "------[${file}]------"
	output=`./bin/darwin/chpl ${file} 2>&1`
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
echo
echo "${success} out of ${total} passed."
