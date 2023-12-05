/*
  This test checks the 'distBag.addBulk()' method.
*/

use DistributedBag;

config const nElems = 1000;
const defaultMaxSegmentCap = DistributedBag.distributedBagMaxSegmentCap;

var bag = new distBag(int);

// Check the number of elements inserted.
const successfulInsertions = bag.addBulk(1..nElems, 0);
assert(successfulInsertions == nElems);
assert(bag.getSize() == nElems);

// Test the limit of the segments.
const successfulInsertions2 = bag.addBulk(1..defaultMaxSegmentCap, 0);
assert(successfulInsertions2 == defaultMaxSegmentCap - nElems);
assert(bag.getSize() == defaultMaxSegmentCap);

writeln("SUCCESS");
