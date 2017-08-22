use DistributedDeque;

// We test both FIFO and LIFO capabilities of the Deque, as well as total ordering.
// This test differs from DequeAccumulator in that it is entirely sequential and
// that the outcome should be deterministic.

config param isBounded = false;

const nElems = 1000;
var deque = (if isBounded then new DistributedDeque(int, cap = nElems) else new DistributedDeque(int));

// FIFO
for i in 1 .. nElems {
  deque.enqueue(i);
}

assert(deque.size == nElems);
assert(deque.contains(nElems / 2));

var expect = 1;
for elem in deque.these(Ordering.FIFO) {
  assert(elem == expect);
  expect += 1;
}

for i in 1 .. nElems {
  var (hasElem, elem) = deque.dequeue();
  assert(hasElem && elem == i);
}
var hasElem = deque.dequeue()[1];
assert(deque.size == 0 && deque.isEmpty() && !hasElem);

// LIFO
for i in 1 .. nElems {
  deque.push(i);
}
assert(deque.size == nElems);
assert(deque.contains(nElems / 2));

expect = nElems;
for elem in deque.these(Ordering.LIFO) {
  assert(elem == expect);
  expect -= 1;
}

for i in 0 .. #nElems {
  var (hasElem, elem) = deque.pop();
  assert(hasElem && elem == nElems - i);
}
hasElem = deque.dequeue()[1];
assert(deque.size == 0 && deque.isEmpty() && !hasElem);

// Total Ordering
// In this case, we add the middle element, then all elements preceeding it to the
// beginning, and all elements succeeding it to the end.
var middle = nElems / 2;
deque.add(middle);
for i in 1 .. middle - 1 {
  deque.pushFront(middle - i);
}
for i in middle + 1 .. nElems {
  deque.pushBack(i);
}
assert(deque.size == nElems);
for i in 1 .. nElems {
  var (hasElem, elem) = deque.dequeue();
  assert(hasElem && elem == i);
}

writeln("SUCCESS");
