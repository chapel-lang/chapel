private use List;

config const testIters = 8;

var lst1: list(int) = 1..testIters;

var lst2: list(int);

assert(lst2.insert(0, lst1));

for (x, y) in zip(lst1, lst2) do
  assert(x == y);

assert(lst2.insert(lst2.size, lst1));

assert(lst2.size == lst1.size * 2);

for i in 0..#testIters do
  assert(lst1[i] == lst2[i]);

for i in 0..#testIters do
  assert(lst1[i] == lst2[testIters + i]);

assert(!lst2.insert(-1, lst1));

assert(lst2.insert(testIters-1, lst1));

