private use UnrolledLinkedList;

config const testIters = 8;

var lst: unrolledLinkedList(int, true);

for i in 1..testIters do
  lst.append(i);

assert(!lst.isEmpty());

ref elem = lst.last();

assert(elem == testIters);

