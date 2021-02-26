private use Vector;

config const testIters = 8;

var arr: [0..#testIters] int = 1..testIters;

var lst: vector(int);

for i in 1..testIters do
  lst.append(i);

for (x, y) in zip(arr, lst) do
  assert(x == y);

assert(lst.insert(lst.size, arr));

assert(lst.size == 2 * arr.size);

for i in 0..#testIters do
  assert(arr[i] == lst[i]);

assert(!lst.insert(-1, arr));

assert(lst.insert(lst.size-1, arr));

assert(lst.size == 3 * arr.size);

