private use List;

config const testIters = 8;

var arr: [1..testIters] int = 1..testIters;

var lst = new list(arr);

for (x, y) in zip(arr, lst) do
  assert(x == y);


