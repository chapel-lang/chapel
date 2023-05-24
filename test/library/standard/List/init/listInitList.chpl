private use List;

config const testIters = 8;

var lst1: list(int);

for i in 1..testIters do
  lst1.pushBack(i);

var lst2 = new list(lst1);

for (x, y) in zip(lst1, lst2) do
  assert(x == y);

lst1.clear();

assert(!lst2.isEmpty());
