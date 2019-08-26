private use List;

config const testIters = 8;

const arr: [1..testIters] int = 1..testIters;
var lst: list(int) = arr;

forall (x, y) in zip(arr, lst) do
  assert(x == y);

forall (x, y) in zip(lst, arr) do
  assert(x == y);



