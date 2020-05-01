private use List;

config const testIters = 8;

var lst: list(int) = 1..testIters;

for i in 1..testIters do
  assert(lst.contains(i));

for i in -1..-testIters do
  assert(!lst.contains(i));

lst.clear();

for i in 1..testIters do
  assert(!lst.contains(i));

