private use UnrolledLinkedList;

config const testIters = 16;

var lst = new unrolledLinkedList(int, true);

for i in 1..testIters do
  lst.append(i);

for i in 1..testIters by -1 {
  const elem = lst.pop();
  assert(elem == i);
}

