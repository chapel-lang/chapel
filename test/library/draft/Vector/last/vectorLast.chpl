private use List;

config const testIters = 8;

var lst: list(int, parSafe=false);

for i in 1..testIters do
  lst.pushBack(i);

assert(!lst.isEmpty());

ref elem = lst.last();

assert(elem == testIters);
