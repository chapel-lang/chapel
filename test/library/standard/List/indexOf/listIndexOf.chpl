private use List;

config const testIters = 8;

var lst: list(int, true);
var idx = 0;

for i in 1..testIters {
  lst.pushBack(i);
  idx = lst.find(i);
  assert(idx == i-1);
}

for i in 1..testIters do
  lst.pushBack(i);

// Value of `end` < 0 defaults to searching entire list.
idx = lst.find(testIters, testIters, -1);

assert(idx >= 0);

lst.popBack();

idx = lst.find(testIters, testIters);

assert(idx < 0);
