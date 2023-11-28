/*
  This test checks the 'distBag.addBulk()' method.
*/

use DistributedBag;

config param nElems = 1000;
config param defaultMaxSegmentCap = 1024*1024;

var bag = new distBag(int);

// Check the number of elements inserted.
var successfulInsertions = bag.addBulk(1..nElems, 0);
assert(successfulInsertions == nElems);
assert(bag.getSize() == nElems);

// Test the limit of the segments (their maximum capacity is 1024*1024).
var successfulInsertions2 = bag.addBulk(1..defaultMaxSegmentCap, 0);
assert(successfulInsertions2 == defaultMaxSegmentCap - nElems);
assert(bag.getSize() == defaultMaxSegmentCap);

writeln("SUCCESS");
