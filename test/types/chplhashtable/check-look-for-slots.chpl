config const verbose = false;

use ChapelHashtable;

var ht: chpl__hashtable(int, nothing);

// How many buckets can lookForSlots check?
// Let's find out.
// Theoretically, quadratic probing should enumerate 1/2 of the slots.
// It should always returns a value in 0..#numSlots

for hash in (max(int)-3, max(int)-2, max(int)-1, max(int), 0, 1, 2, 3) {
  for numSlots in (3, 7, 11, 19, 23, 31, 47, 83, 191, 383) {
    var hits:[0..#numSlots] int;
    for i in ht._lookForSlots(hash, numSlots) {
      if verbose then
        writeln("lookForSlots(", hash, ",", numSlots, ") yielded ", i);
      assert( 0 <= i && i < numSlots );
      hits[i] += 1;
    }
    var fullSlots = 0;
    for i in 0..#numSlots {
      if verbose then
        writeln("hits[", i, "] = ", hits[i]);
      if hits[i] > 0 then fullSlots += 1;
    }
    if verbose then
      writeln("lookForSlots(", hash, ",", numSlots, ") resulted in ", fullSlots,
              " full slots");
    assert(fullSlots >= numSlots / 2);
  }
}

