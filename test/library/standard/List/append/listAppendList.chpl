private use List;

config const testIters = 8;

var lst1: list(int) = 1..testIters;
var lst2: list(int, true);

lst2.pushBack(lst1);

for (x, y) in zip(lst1, lst2) do
  assert(x == y);

lst2.pushBack(lst1);

assert(lst2.size > lst1.size);

for i in 0..#testIters {
  var v1 = lst1.getValue(i);
  var v2 = lst2.getValue(testIters + i);
  assert(v1 == v2);
}
