private use List;

config const testIters = 8;

const r = 1..testIters;

var lst = new list(r);

for (x, y) in zip(r, lst) do
  assert(x == y);

