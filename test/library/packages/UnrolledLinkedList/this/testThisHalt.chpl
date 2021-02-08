private use UnrolledLinkedList;

config const testIters = 8;

var lst: unrolledLinkedList(int, true);

for i in 1..testIters do
  lst[i] *= 2;

