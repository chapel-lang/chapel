#include "rand.h"
#include <math.h>
#include <stdio.h>

rndtype m2[64];
static void computeM2Vals(rndtype m[]);

static void computeM2Vals(rndtype m[]) {
  rndtype nextVal = 0x1;
  int i;

  for (i=0; i < 64; i++) {
    m[i] = nextVal;
    getNextRandom(&nextVal);
    getNextRandom(&nextVal);
  }
}

void randInit() {
  computeM2Vals(m2);
}

void getNextRandom(rndtype* x) {
  const rndtype poly = 0x7;
  const rndtype hiRandBit = 0x1ll << (64 - 1);
  if (*x & hiRandBit)
    *x = (*x << 1) ^ poly;
  else
    *x = (*x << 1);
}

// Jump the random number to the nth position
rndtype getNthRandom(rndtype n) {
  const rndtype period = 0x7fffffffffffffffull / 7;
  rndtype ran = 0x2;
  int i;

  n %= period;
  if (n == 0)
    return 0x1;
  for (i = ((int)(log(n)/log(2)))-1; i >= 0; i--) {
    rndtype val = 0;
    int j;
    for (j=0; j<64; j++) {
      if ((ran >> j) & 1)
        val ^= m2[j];
    }
    ran = val;
    if ((n >> i) & 1)
      getNextRandom(&ran);
  }
  return ran;
}

