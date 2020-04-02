//
// A helper module for the RA benchmark that defines the random stream
// of values
//
module RARandomStream {
  param randWidth = 64;              // the bit-width of the random numbers
  type randType = uint(randWidth);   // the type of the random numbers

  //
  // bitDom is a non-distributed domain whose indices correspond to
  // the bit positions in the random values.  m2 is a table of helper
  // values used to fast-forward through the random stream.
  //
  const bitDom = {0..#randWidth},
        m2: [bitDom] randType = computeM2Vals(randWidth);

  //
  // A serial iterator for the random stream that resets the stream
  // to its 0th element and yields values endlessly.
  //
  iter RAStream() {
    var val = getNthRandom(0);
    while (1) {
      getNextRandom(val);
      yield val;
    }
  }

  //
  // A "follower" iterator for the random stream that takes a range of
  // 0-based indices (follower) and yields the pseudo-random values
  // corresponding to those indices.  Follower iterators like these
  // are required for parallel zippered iteration.
  //
  iter RAStream(param tag: iterKind, followThis) where tag == iterKind.follower {
    var val = getNthRandom(followThis(0).low);
    for zip((...followThis)) {
      getNextRandom(val);
      yield val;
    }
  }

  //
  // A helper function for "fast-forwarding" the random stream to
  // position n in O(log2(n)) time
  //
  proc getNthRandom(in n: uint(64)) {
    param period = 0x7fffffffffffffff/7;
    var ret: randType;
    n %= period;
    if (n == 0) then return 0x1;

    on Locales(0) {
      var ran: randType = 0x2;
      for i in 0..log2(n)-1 by -1 {
        var val: randType = 0;
        for j in 0..#randWidth do
          if ((ran >> j) & 1) then val ^= m2(j);
        ran = val;
        if ((n >> i) & 1) then getNextRandom(ran);
      }
      ret = ran;
    }
    return ret;
  }

  //
  // A helper function for advancing a value from the random stream,
  // x, to the next value
  //
  proc getNextRandom(inout x) {
    param POLY = 0x7;
    param hiRandBit = 0x1:randType << (randWidth-1);

    x = (x << 1) ^ (if (x & hiRandBit) then POLY else 0);
  }

  //
  // A helper function for computing the values of the helper array,
  // m2
  //
  iter computeM2Vals(numVals) {
    var nextVal = 0x1: randType;
    for i in 1..numVals {
      yield nextVal;
      getNextRandom(nextVal);
      getNextRandom(nextVal);
    }
  }
}
