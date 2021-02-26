private use UnrolledLinkedList;

config const testIters = 128;

var lst: unrolledLinkedList(int);

for i in 1..testIters do
  lst.append(i);

for i in 1..testIters do
  assert(lst.contains(i));

for i in -testIters..-1 do
  assert(!lst.contains(i));

lst.clear();

for i in 1..testIters do
  assert(!lst.contains(i));

