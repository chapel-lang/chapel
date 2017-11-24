#!/bin/bash

source env.sh

mkdir -p logs

# run small crush suite varying parameters
for testn in $TESTS
do
  rm -f ./$testn-log.txt
  for seedi in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
  do
    rm -f ./logs/$testn-$seedi-log.txt
    rm -f ./logs/$testn-$seedi-summary.txt
    ./$testn --seedi=$seedi > ./logs/$testn-$seedi-log.txt
    grep -A5000 -m1 -e "========= Summary results of " ./logs/$testn-$seedi-log.txt > ./logs/$testn-$seedi-summary.txt
    grep -q "All tests were passed" ./logs/$testn-$seedi-summary.txt
    if [ $? != 0 ]
    then
      NFAIL=`cat ./logs/$testn-$seedi-summary.txt | grep -e '^[ ]*[0-9]' | wc -l`
      echo "$testn seedi=$seedi failures: $NFAIL"
      echo "$testn seedi=$seedi failures: $NFAIL" >> ./$testn-log.txt
      cat ./logs/$testn-$seedi-summary.txt >> ./$testn-log.txt
    else
      echo "$testn seedi=$seedi pass"
      echo "$testn seedi=$seedi pass" >> ./$testn-log.txt
    fi
  done
done


