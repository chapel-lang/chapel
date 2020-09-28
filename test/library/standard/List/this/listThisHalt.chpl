private use List;

config const testIters = 8;

var lst: list(int, parSafe=false);

for i in 1..testIters do
  lst[i] *= 2;

