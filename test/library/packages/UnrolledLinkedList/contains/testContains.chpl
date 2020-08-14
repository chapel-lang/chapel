private use UnrolledLinkedList;

config const testIters = 8;

var lst: unrolledLinkedList(int);

for i in 1..testIters do
  lst.append(i);

for i in 1..testIters do
  assert(lst.contains(i));

for i in -1..-testIters do
  assert(!lst.contains(i));

lst.clear();

for i in 1..testIters do
  assert(!lst.contains(i));

