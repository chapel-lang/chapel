#!/bin/bash

# This script shows a problem with Cygwin where
# unlinking/removing an executable that used stdout
# will occasionally result in a Device or resource busy
# error.

# This error occurs about 10/1000 times on cygwin32.

# When run, this script will only print out the error cases.

# It appears that the executable must use stdout to cause the problem.

echo '
#include <stdio.h>
int main(int argc, char** argv) {
  printf("Hello\n");
  return 0;
}' > return0.c
gcc return0.c -o return0

for i in `seq 1 1000`
do
cp return0 a.out && ./a.out > /dev/null && rm a.out
done
