use Barriers;
use DistributedDeque;
use DistributedBag;

// For this test, we implement a simple counter; we add a predetermined number
// of elements, then concurrently remove all elements from it until it is empty,
// testing that all elements added were removed.
config param isBoundedDeque = false;
config param isDeque = false;
config param isBag = false;

config param nElems = 1000;
const expected = (nElems * (nElems + 1)) / 2;

var c = (
  if isBoundedDeque then new DistDeque(int, cap=nElems)
  else if isDeque then new DistDeque(int)
  else if isBag then new DistBag(int)
  else compilerError("Require 'isBoundedDeque', 'isDeque', or 'isBag' to be set...")
);


// Fill...
for i in 1 .. nElems {
  c.add(i);
}

assert(c.getSize() == nElems);
assert(c.contains((nElems / 2) : int));

// Iterate over the collection.
var actual = 0;
for elem in c {
  actual = actual + elem;
}
assert(actual == expected);
assert(c.getSize() == nElems);

// Iterate concurrently over the collection.
var concurrentActual : atomic int;
forall elem in c {
  concurrentActual.add(elem);
}
assert(concurrentActual.read() == expected);
assert(c.getSize() == nElems);

// Empty collection. Make sure all tasks start around same time...
if isBag then c.balance();
concurrentActual.write(0);
var barrier = new Barrier(here.maxTaskPar * numLocales);
coforall loc in Locales do on loc {
  var perLocaleActual : atomic int;
  const _c = c;

  coforall tid in 0..#here.maxTaskPar {
    barrier.barrier();
    // BUG: Moving this declaration above the loop results in an incorrect
    // compiler warning: 'A while loop with a constant condition'
    var (hasElem, elt) : (bool, int) = (true, 0);
    var perTaskActual : int;
    while hasElem {
      perTaskActual = perTaskActual + elt;
      (hasElem, elt) = _c.remove();
    }
    perLocaleActual.add(perTaskActual);
  }
  concurrentActual.add(perLocaleActual.read());
}

assert(concurrentActual.read() == expected);
assert(c.getSize() == 0 && c.isEmpty());
writeln("SUCCESS");
