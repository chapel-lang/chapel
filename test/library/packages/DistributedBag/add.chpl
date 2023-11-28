/*
  This test checks the 'distBag.add()' method.
*/

use DistributedBag;

config param nElems = 3;
config param defaultMaxSegmentCap = 1024*1024;

var bag = new distBag(int);

// Check the number of elements inserted.
for i in 1..nElems do bag.add(i, 0);
assert(bag.getSize() == nElems);

// Test the limit of the segments (their maximum capacity is 1024*1024).
for i in 1..defaultMaxSegmentCap do bag.add(i, 0);
assert(bag.getSize() == defaultMaxSegmentCap);

writeln("SUCCESS");
