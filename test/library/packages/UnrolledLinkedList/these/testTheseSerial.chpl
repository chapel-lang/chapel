private use UnrolledLinkedList;

config const testIters = 8;

const arr: [1..testIters] int = 1..testIters;

var lst = new unrolledLinkedList(arr, true);

for (x, y) in zip(arr, lst) do
  assert(x == y);

lst.clear();

var count = 0;

for x in lst do
  count += 1;

assert(count == 0);

