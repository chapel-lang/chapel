private use List;

config const testIters = 137;

var lst1: list(int);
var lst2: list(int, true);

for i in 1..testIters do
  lst1.pushBack(i);

forall x in lst1 do
  x *= 2;

// List 2 has `parSafe=true`, here.
forall x in lst1 with (ref lst2) do
  lst2.pushBack(x);

lst2.sort();

for (x, y) in zip(lst1, lst2) do
  assert(x == y);
