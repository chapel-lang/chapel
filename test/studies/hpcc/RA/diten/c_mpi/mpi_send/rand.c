#include "rand.h"
#include <math.h>
#include <stdio.h>

u64 m2[64];
static void computeM2Vals(u64 m[]);

static void computeM2Vals(u64 m[]) {
  u64 nextVal = 0x1;
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

void getNextRandom(u64* x) {
  const u64 poly = 0x7;
  const u64 hiRandBit = 0x1ll << (64 - 1);
  if (*x & hiRandBit)
    *x = (*x << 1) ^ poly;
  else
    *x = (*x << 1);
}

// Jump the random number to the nth position
u64 getNthRandom(u64 n) {
  const u64 period = 0x7fffffffffffffffull / 7;
  u64 ran = 0x2;
  int i;

  n %= period;
  if (n == 0)
    return 0x1;
  for (i = ((int)(log(n)/log(2)))-1; i >= 0; i--) {
    u64 val = 0;
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

