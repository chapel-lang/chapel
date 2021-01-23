private use UnrolledLinkedList;

config const testIters = 120;

var lst: unrolledLinkedList(int, true);
var idx = 0;

for i in 1..testIters {
  lst.append(i);
}

for i in 1..testIters {
  idx = lst.indexOf(i);
  assert(idx == i-1);
}

for i in 1..testIters do
  lst.append(i);

// Value of `end` < 0 defaults to searching entire list.
idx = lst.indexOf(testIters, testIters, -1);

assert(idx >= 0);

lst.pop();

idx = lst.indexOf(testIters, testIters);

assert(idx < 0);

