private use List;

config const testIters =8;

var lst: list(int);

for i in 1..testIters do
  lst.pushBack(i);

const elem = lst.pop(testIters + 1);
