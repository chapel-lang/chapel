use Barriers;
use DistributedDeque;

// For the parity test, we separate even and odd numbers on each side of the Deque.
// Even numbers are inserted from the front, and odd numbers are inserted from the back.
// This is performed concurrently across nodes; this ensures that the Deque is concurrent
// safe and upholds its ordering at all times.

// If we are testing a bounded queue...
config param isBounded = false;

config param nElemsPerTask = 100;
const totalElems = numLocales * here.maxTaskPar * nElemsPerTask;

var cap : int;
if isBounded {
  cap = totalElems;
} else {
  cap = -1;
}

var deque = new DistDeque(int, cap=cap);
var barrier = new Barrier(numLocales * here.maxTaskPar);

// Concurrent add... Ensure they all start around same time...
coforall loc in Locales do on loc {
  coforall tid in 0..#here.maxTaskPar {
    barrier.barrier();
    for i in 1 .. nElemsPerTask {
      // Even...
      if i % 2 == 0 {
        deque.pushFront(i);
      } else {
        deque.pushBack(i);
      }
    }
  }
}

// Half of the numbers are even, the other half are odd... Concurrent remove
// half, expecting it to be even, then the other half expecting it to be odd...
forall 1 .. totalElems / 2 {
  const (hasElem, elem) = deque.popFront();
  assert(hasElem && elem % 2 == 0);
}
forall 1 .. totalElems / 2 {
  const (hasElem, elem) = deque.popFront();
  assert(hasElem && elem % 2 != 0);
}

writeln("SUCCESS");
