/*
  This test checks the 'distBag.addBulk()' method.
*/

use DistributedBag;

config const nElems = 1000;

// Test 1: check the number of elements inserted and the capacity limit of the segments.
{
  const defaultMaxSegmentCap = DistributedBag.distributedBagMaxSegmentCap;

  var bag = new distBag(int);

  // Check the number of elements inserted.
  const successfulInsertions = bag.addBulk(1..nElems, 0);
  assert(successfulInsertions == nElems);
  assert(bag.getSize() == nElems);

  // Test the capacity limit of the segments.
  const successfulInsertions2 = bag.addBulk(1..defaultMaxSegmentCap, 0);
  assert(successfulInsertions2 == defaultMaxSegmentCap - nElems);
  assert(bag.getSize() == defaultMaxSegmentCap);

  writeln("Test 1: SUCCESS");
}

// Test 2: check the elements inserted (array slice)
{
  const A: [0..9] int = [0,1,2,3,4,5,6,7,8,9];

  var bag = new distBag(int);

  bag.addBulk(A[2..9], 0);
  writeln("Output test 2: ", bag);
}
