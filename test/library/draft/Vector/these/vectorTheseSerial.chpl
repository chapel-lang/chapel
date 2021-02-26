private use Vector;

config const testIters = 8;

const arr: [1..testIters] int = 1..testIters;

var vec: vector(int, true) = arr;

for (x, y) in zip(arr, vec) do
  assert(x == y);

vec.clear();

var count = 0;

for x in vec do
  count += 1;

assert(count == 0);

