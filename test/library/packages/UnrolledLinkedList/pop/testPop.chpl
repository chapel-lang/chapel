private use UnrolledLinkedList;

config const testIters = 64;

var lst = new unrolledLinkedList(int, true);

for i in 1..testIters do
  lst.append(i);

assert(lst.pop(0) == 1);

for i in 2..testIters by -1 {
  const elem = lst.pop();
  assert(elem == i);
}

for i in 1..testIters do
  lst.append(i);

assert(lst.pop(0) == 1);

for i in 2..testIters by -1 {
  const elem = lst.pop();
  assert(elem == i);
}
