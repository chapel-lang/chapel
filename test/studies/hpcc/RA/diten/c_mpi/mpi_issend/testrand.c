#include <stdio.h>
#include "rand.h"

int main (int argc, char* argv[]) {
  int i;
  rndtype r;
  randInit();
  r = getNthRandom(1);
  for (i=0; i<10; i++) {
    printf("%d %llu\n", i, r);
    getNextRandom(&r);
  }

  r = getNthRandom(123456789llu);
  for (i=0; i<10; i++) {
    printf("%d %llu\n", i, r);
    getNextRandom(&r);
  }
  return 0;
}
