/*
  This test checks the 'distBag.add()' method.
*/

use DistributedBag;

config const nElems = 3;
const defaultMaxSegmentCap = DistributedBag.distributedBagMaxSegmentCap;

var bag = new distBag(int);

// Check the number of elements inserted.
for i in 1..nElems {
  const res = bag.add(i, 0);
  assert(res == true);
}
assert(bag.getSize() == nElems);

// Test the limit of the segments.
var c = 0;
for i in 1..defaultMaxSegmentCap {
  const res = bag.add(i, 0);
  if !res then c += 1; // count the failures
}
assert(c == nElems);
assert(bag.getSize() == defaultMaxSegmentCap);

writeln("SUCCESS");
