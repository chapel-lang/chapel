use DistributedDeque;

// We test both FIFO and LIFO capabilities of the Deque, as well as total ordering.
// This test differs from DequeAccumulator in that it is entirely sequential and
// that the outcome should be deterministic.

config param isBounded = false;

const nElems = 1000;
var deque = (if isBounded then new DistDeque(int, cap = nElems) else new DistDeque(int));

// First-In-First-Out Test
// We add elements in a queue-like fashion, testing queue capabilities.
for i in 1 .. nElems {
  var inserted = deque.enqueue(i);
  assert(inserted == true);
}

// Sanity Check: Ensure size is correct
assert(deque.size == nElems);
// Sanity Check: Ensure we can find middle item...
assert(deque.contains(nElems / 2));
// Sanity Check: Ensure bounds checking is working, if is bounded.
if isBounded then assert(deque.enqueue(nElems + 1) == false);

// First-In-First-Out Check [Pt. 1]
// Since we added elements in queue-like fashion, we should receive it in a queue-like fashion.
// This tests the serial iterator.
var expect = 1;
for elem in deque.these(Ordering.FIFO) {
  assert(elem == expect);
  expect += 1;
}

// First-In-First-Out Check [Pt. 2]
// Since we added elements in queue-like fashion, we should receive it in a queue-like fashion.
// This tests the removal operations...
var (hasElem, elem) : (bool, int);
for i in 1 .. nElems {
  (hasElem, elem) = deque.dequeue();
  assert(hasElem && elem == i);
}

// Sanity Check: Ensures size is correct
assert(deque.size == 0 && deque.isEmpty());
// Sanity Check: Ensures that we handle being empty.
var isEmpty = !deque.dequeue()[1];
assert(deque.size == 0 && deque.isEmpty() && isEmpty);

// Last-In-First-Out Test
// We add elements in a stack-like fashion, testing stack capabilities.
for i in 1 .. nElems {
  deque.push(i);
}

// Sanity Check: Ensure size is correct
assert(deque.size == nElems);
// Sanity Check: Ensure we can find middle item...
assert(deque.contains(nElems / 2));
// Sanity Check: Ensure bounds checking is working, if is bounded.
if isBounded then assert(deque.enqueue(nElems + 1) == false);

// Last-In-First-Out Check [Pt. 1]
// Since we added elements in stack-like fashion, we should receive it in a stack-like fashion.
// This tests the serial iterator.
expect = nElems;
for elem in deque.these(Ordering.LIFO) {
  assert(elem == expect);
  expect -= 1;
}

// Last-In-First-Out Check [Pt. 2]
// Since we added elements in stack-like fashion, we should receive it in a stack-like fashion.
// This tests the removal operations...
for i in 0 .. #nElems {
  (hasElem, elem) = deque.pop();
  assert(hasElem && elem == nElems - i);
}

// Sanity Check: Ensures size is correct
assert(deque.size == 0 && deque.isEmpty());
// Sanity Check: Ensures that we handle being empty.
hasElem = deque.pop()[0];
assert(deque.size == 0 && deque.isEmpty() && !hasElem);

// Total Ordering Test
// In this case, we add the middle element, then all elements preceeding it to the
// beginning, and all elements succeeding it to the end. This tests insertions to and
// from both ends of the deque.
var middle = nElems / 2;
deque.add(middle);
for i in 1 .. middle - 1 {
  deque.pushFront(middle - i);
}
for i in middle + 1 .. nElems {
  deque.pushBack(i);
}

// Sanity Check: Ensure size is correct
assert(deque.size == nElems);
// Sanity Check: Ensure we can find middle item...
assert(deque.contains(nElems / 2));
// Sanity Check: Ensure bounds checking is working, if is bounded.
if isBounded then assert(deque.pushBack(nElems + 1) == false && deque.pushBack(nElems + 1) == false);

// Total Ordering Check [Pt. 1]
// Since the end result is similar to if we inserted it in First-In-First-Out order (1..nElems), wee
// should be able to iterate over it in FIFO order.
expect = 1;
for elem in deque.these(Ordering.FIFO) {
  assert(elem == expect);
  expect += 1;
}

// Total Ordering Check [Pt. 2]
// Following the above logic, we should also be capable of iterating over it in reverse in LIFO order.
expect = nElems;
for elem in deque.these(Ordering.LIFO) {
  assert(elem == expect);
  expect -= 1;
}

// Total Ordering Check [Pt. 3]
// Finally, we should be able to remove elements in FIFO order.
for i in 1 .. nElems {
  (hasElem, elem) = deque.dequeue();
  assert(hasElem && elem == i);
}

writeln("SUCCESS");
