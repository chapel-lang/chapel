/*
  This test checks the 'distBag.addBulk()' method.
*/

use DistributedBag;

config const nElems = 1000;

// Test 1: checks the number of elements inserted and the capacity limit of the segments.
{
  const defaultMaxSegmentCap = DistributedBag.distributedBagMaxSegmentCap;

  // Saturates a segment at once while it is empty.
  {
    var bag = new distBag(int);

    const successfulInsertions = bag.addBulk(1..(2*defaultMaxSegmentCap), 0);
    assert(successfulInsertions == defaultMaxSegmentCap);
    assert(bag.getSize() == defaultMaxSegmentCap);
  }

  // Inserts some elements into a segment, and then saturates it.
  {
    var bag = new distBag(int);

    const successfulInsertions = bag.addBulk(1..nElems, 0);
    assert(successfulInsertions == nElems);
    assert(bag.getSize() == nElems);

    const successfulInsertions2 = bag.addBulk(1..defaultMaxSegmentCap, 0);
    assert(successfulInsertions2 == defaultMaxSegmentCap - nElems);
    assert(bag.getSize() == defaultMaxSegmentCap);
  }

  writeln("Test 1: SUCCESS");
}

// Test 2: checks the elements inserted using an array or range.
{
  const A: [0..9] int = [0,1,2,3,4,5,6,7,8,9];
  const r = 0..9;

  // Inserts an array slice.
  {
    var bag = new distBag(int);

    bag.addBulk(A[2..9], 0);
    writeln("Output test 2a: ", bag);
  }

  // Inserts strided range.
  {
    var bag = new distBag(int);

    bag.addBulk(r[0.. by 3], 0);
    writeln("Output test 2b: ", bag);
  }
}
