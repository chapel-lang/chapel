
//
// A helper module for the RA benchmark that defines the random stream
// of values
//
module RARandomStream {
  config param useLCG = true;

  param randWidth = 64;              // the bit-width of the random numbers
  type randType = uint(randWidth);   // the type of the random numbers


  //
  // m2 is a table (tuple) of helper values used to fast-forward
  // through the random stream.
  //
  const m2: randWidth*randType = computeM2Vals();

  //
  // A serial iterator for the random stream that resets the stream
  // to its 0th element and yields values endlessly.
  //
  iter RAStream() {
    var val = getNthRandom(0:randType);
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
    if followThis.size != 1 then
      compilerError("RAStream cannot use multi-dimensional iterator");
    const (followInds,) = followThis;
    var val = getNthRandom(followInds.low);
    for zip((...followThis)) {
      getNextRandom(val);
      yield val;
    }
  }

  //
  // magic number for the LCG random stream
  //

  //
  // A helper function for "fast-forwarding" the random stream to
  // position n in O(log2(n)) time
  //
  proc getNthRandom(in n: uint(64)) {
    if (useLCG) {
      var mul_k = 6364136223846793005:randType,
          add_k = 1:randType;

      var ran = 1:randType;
      while (n) {
	if (n & 1) then
	  ran = mul_k * ran + add_k;
	add_k *= (mul_k + 1);
	mul_k *= mul_k;
	n >>= 1;
      }
      return ran;
    } else {
      param period = 0x7fffffffffffffff/7;

      n %= period;
      if (n == 0) then return 0x1;
      var ran: randType = 0x2;
      for i in 0..log2(n)-1 by -1 {
	var val: randType = 0;
	for j in 0..<randWidth do
	  if ((ran >> j) & 1) then val ^= m2(j);
	ran = val;
	if ((n >> i) & 1) then getNextRandom(ran);
      }
      return ran;
    }
  }

  //
  // A helper function for advancing a value from the random stream,
  // x, to the next value
  //
  proc getNextRandom(inout x) {
    if (useLCG) {
      x = 6364136223846793005*x + 1;
    } else {
      param POLY = 0x7;
      param hiRandBit = 0x1:randType << (randWidth-1);

      x = (x << 1) ^ (if (x & hiRandBit) then POLY else 0);
    }
  }

  //
  // A helper function for computing the values of the helper tuple, m2
  //
  proc computeM2Vals() {
    var m2tmp: randWidth*randType;
    var nextVal = 0x1: randType;
    for i in m2tmp.indices {
      m2tmp(i) = nextVal;
      getNextRandom(nextVal);
      getNextRandom(nextVal);
    }
    return m2tmp;
  }
}
