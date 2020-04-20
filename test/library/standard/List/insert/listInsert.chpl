private use List;

config const testIters = 16;
config const testElem = 42;

var lst: list(int);

for i in 1..testIters do
  lst.append(i);

for i in 1..testIters do
  assert(lst.insert(0, testElem));

var oldSize = lst.size;

// Insert at -1 should fail.
assert(!lst.insert(-1, testElem));
assert(lst.size == oldSize);

// Insert at size should always work.
assert(lst.insert(lst.size, testElem));

lst.clear();
assert(lst.size == 0);

assert(lst.insert(0, testElem));

