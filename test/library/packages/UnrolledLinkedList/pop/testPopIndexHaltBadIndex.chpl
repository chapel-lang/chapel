private use UnrolledLinkedList;

config const testIters =8;

var lst: unrolledLinkedList(int);

for i in 1..testIters do
  lst.append(i);

const elem = lst.pop(testIters + 1);

