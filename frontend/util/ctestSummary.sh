#!/usr/bin/env bash

CHPL_MAKE_HOME=$1
ctest_exit_code=$2

log=${CHPL_MAKE_HOME}/build/frontend-test/Testing/Temporary/LastTestsFailed.log
numFailures=$(wc -l < $log)

echo ""
echo "Testing summary: $numFailures Failures:"

for line in $(cat $log); do
  testName=$(cut -d ':' -f 2 <<< $line)
  fname=$(find ${CHPL_MAKE_HOME}/frontend/test -name "$testName.cpp" | head -n 1)
  echo $fname
done

echo

exit $ctest_exit_code
