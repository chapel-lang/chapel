#include "externMethodCallPerf.h"

int blah2(int num) {
  int res = num % 17;
  res = res + 4;
  res = res * 3;
  res = res - 7;
  return res/8;
}
