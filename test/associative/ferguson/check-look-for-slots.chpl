config const verbose = false;


// new version
iter lookForSlots(hash:int, numSlots:int) {
  // note - defaultHashWrapper returns a positive int,
  // and so baseSlot should be positive and in 0..#numSlots
  const baseSlot = hash % numSlots;
  for probe in 0..#numSlots {
    // quadratic probing
    // alternating sign allows hashtable to be > 50% full
    // and to still find empty slots
    var neg = (probe & 1) != 0;
    // now compute a positive offset
    // in the negative case, we are modding a negative number,
    // which might result in a value < 0; so we add numSlots to
    // keep it positive.
    var squareProbe =
      if neg then -probe*probe
      else probe*probe;
    var offset = numSlots + squareProbe % numSlots;
    // Now baseSlot and offset are both in 0..2*numSlots
    // so adding them shouldn't result in overflow
    // (except for a very very large table)
    yield (baseSlot + offset) % numSlots;
  }
}

// How many buckets can lookForSlots check?
// Let's find out.
// Theoretically, it should enumerate 1/2 of the slots.
// It should always returns a value in 0..#numSlots

for hash in (max(int)-3, max(int)-2, max(int)-1, max(int), 0, 1, 2, 3) {
  for numSlots in (3, 7, 11, 19, 23, 31, 47, 83, 191, 383) {
    var hits:[0..#numSlots] int;
    for i in lookForSlots(hash, numSlots) {
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
    assert(fullSlots == numSlots);
  }
}

