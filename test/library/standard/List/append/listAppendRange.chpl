private use List;

config const testIters = 8;

var lst: list(int, true);

const r = 1..testIters;

lst.append(r);

for (x, y) in zip(r, lst) do
  assert(x == y);


