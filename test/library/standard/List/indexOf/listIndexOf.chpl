private use List;

config const testIters = 8;

var lst: list(int, true);

var idx = lst.indexOf(testIters);

assert(idx < 0);

idx = lst.indexOf(testIters, -1);

assert(idx < 0);

idx = lst.indexOf(testIters, 1, lst.size);

assert(idx < 0);

for i in 1..testIters {
  lst.append(i);
  idx = lst.indexOf(i);
  assert(idx == i);
}

for i in 1..testIters do
  lst.append(i);

idx = lst.indexOf(testIters, testIters + 1);

assert(idx > testIters);

idx = lst.indexOf(testIters, 1, testIters - 1);
assert(idx < 0);

idx = lst.indexOf(testIters, testIters + 1, testIters * 2 - 1);
assert(idx < 0);

lst.clear();

idx = lst.indexOf(testIters);
assert(idx < 0);

idx = lst.indexOf(testIters, testIters, testIters * 2);
assert(idx < 0);

