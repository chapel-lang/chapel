private use UnrolledLinkedList;
private use List;

config const testIters = 8;

var lst1: list(int, false);
for i in 1..testIters do
  lst1.pushBack(i);

var lst2: unrolledLinkedList(int, false);

lst2.append(lst1);

for (x, y) in zip(lst1, lst2) do
  assert(x == y);

lst2.append(lst1);

assert(lst2.size > lst1.size);

for i in 0..#testIters do
  assert(lst1[i] == lst2[testIters + i]);
