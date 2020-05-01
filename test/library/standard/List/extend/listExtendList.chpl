private use List;

config const testIters = 8;

var lst1: list(int, true) = 1..testIters;
var lst2: list(int, true);

lst2.extend(lst1);

for (x, y) in zip(lst1, lst2) do
  assert(x == y);

lst2.extend(lst1);

assert(lst2.size > lst1.size);

for i in 0..#testIters do
  assert(lst1[i] == lst2[testIters + i]);


