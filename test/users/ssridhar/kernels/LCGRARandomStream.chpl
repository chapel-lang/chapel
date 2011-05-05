//
// A helper module for the RA benchmark that defines the random stream
// of values
//
module RARandomStream {
  param randWidth = 64;              // the bit-width of the random numbers
  type randType = uint(randWidth);   // the type of the random numbers

  param LCGMUL64 = 0x5851F42D4C957F2D;
  param LCGADD64 = 1;

  proc whichRNG(): string {
    return "LCG";
  }

  //
  // A serial iterator for the random stream that resets the stream
  // to its 0th element and yields values endlessly.
  //
  iter RAStream(seed: randType) {
    var val = LCGgetNthRandom(0, seed);
    while (1) {
      LCGgetNextRandom(val);
      yield val;
    }
  }

  //
  // A "follower" iterator for the random stream that takes a range of
  // 0-based indices (follower) and yields the pseudo-random values
  // corresponding to those indices.  Follower iterators like these
  // are required for parallel zippered iteration.
  //
  iter RAStream(param tag: iterator, follower, seed: randType) where tag == iterator.follower {
    if follower.size != 1 then
      halt("RAStream cannot use multi-dimensional iterator");
    var val = LCGgetNthRandom(follower(1).low, seed);
    for follower {
      LCGgetNextRandom(val);
      yield val;
    }
  }

  //
  // A helper function for "fast-forwarding" the random stream to
  // position n in O(log2(n)) time
  //
  proc LCGgetNthRandom(in n: uint(64), seed: randType) {
    var mulk, addk, un: randType;        
    var ran: randType = seed * LCGMUL64;

    mulk = LCGMUL64;
    addk = LCGADD64;
    un = n;

    while (un > 0) {
      if (un & 1) then
	ran = mulk * ran + addk;
      addk *= mulk + 1;
      mulk *= mulk;
      un >>= 1;
    }

    return ran;
  }

  //
  // A helper function for advancing a value from the random stream,
  // x, to the next value
  //
  proc LCGgetNextRandom(inout x: randType) {  
    x = LCGMUL64 * x + LCGADD64;
  }

  pragma "inline"
  proc indexMask(r: randType, n): randType {
    return r >> (64 - n);
  }
}

