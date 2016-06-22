#!/bin/bash

source env.sh

mkdir -p logs

# run big crush suite
for testn in $TESTS
do
    seedi=big
    rm -f ./logs/$testn-$seedi-log.txt
    rm -f ./logs/$testn-$seedi-summary.txt
    ./$testn --crush=true > ./logs/$testn-$seedi-log.txt
    grep -A5000 -m1 -e "========= Summary results of " ./logs/$testn-$seedi-log.txt > ./logs/$testn-$seedi-summary.txt
    grep -q "All tests were passed" ./logs/$testn-$seedi-summary.txt
    if [ $? != 0 ]
    then
      NFAIL=`cat ./logs/$testn-$seedi-summary.txt | grep -e '^[ ]*[0-9]' | wc -l`
      echo "$testn $seedi failures: $NFAIL"
      echo "$testn $seedi failures: $NFAIL" >> ./$testn-log.txt
      cat ./logs/$testn-$seedi-summary.txt >> ./$testn-log.txt
    else
      echo "$testn $seedi pass"
    fi
done


