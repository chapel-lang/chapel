// This random number generator was written from the NPB
// random number generator.  
//
// It's the linear congruential generator with
//
// m = 2^46 (the modulus)
// a = 1220703125.0 (the multiplier)
// c = 0 (the increment)
//
// It requires an odd integer for its seed.

use Time;

config param debugParRandom: bool = false;

class SeedGeneratorClass {
  def clockMS {
    return getCurrentTime(unit=TimeUnits.microseconds):int(64);
  }
}

var SeedGenerator = new SeedGeneratorClass();

class RandomStream {
  const seed:int(64) = SeedGenerator.clockMS; 
  const arand = 1220703125.0;

  // These are meant to be private; internal to class; not set by user
  const r23   = 0.5**23,
        t23   = 2.0**23,
        r46   = 0.5**46,
        t46   = 2.0**46;
  const internalSeed = computeInternalSeed();

  var lock$: sync bool = false;
  var sharedCursor = internalSeed:real;
  var sharedCount: int(64) = 1;

  def computeInternalSeed() {
    var val: int(64) = seed;
    // ensure seed is odd
    if (val % 2 == 0) then val += 1;
    return val;
  }

  def initCursorVal() {
    sharedCursor = internalSeed:real;
    sharedCount = 1;
  }

  def randlc(inout x, a = arand) {
    var t1 = r23 * a;
    const a1 = floor(t1),
          a2 = a - t23 * a1;
    t1 = r23 * x;
    const x1 = floor(t1),
          x2 = x - t23 * x1;
    t1 = a1 * x2 + a2 * x1;
    const t2 = floor(r23 * t1),
          z  = t1 - t23 * t2,
          t3 = t23 * z + a2 * x2,
          t4 = floor(r46 * t3),
          x3 = t3 - t46 * t4;
    x = x3;

    return r46 * x3;
  }

  def getNext() {
    sharedCount += 1;
    return randlc(sharedCursor);
  }

  def skipToNth(in n: integral) {
    if (n <= 0) then 
      halt("RandomStream.skipToNth() can only be called with positive values");
    initCursorVal();
    sharedCount = n;
    n -= 1;
    var t = arand;
    var retval = arand;
    while (n != 0) {
      const i = n / 2;
      if (2 * i != n) then
        randlc(sharedCursor, t);
      retval = randlc(t, t);
      n = i;
    }
  }

  // n is assumed to be 1..
  def getNth(n : integral) {
    if (n <= 0) then 
      halt("RandomStream.getNth() can only be called with a positive value, received: ", n);
    skipToNth(n);
    return getNext();
  }


  def fillRandom(X: [] real) {
    lock$;
    const startCount: int(64) = sharedCount;
    sharedCount = startCount + X.numElements;
    lock$ = false;
    if (X.rank == 1) {
      forall (x, r) in (X, chpl__these(startCount, X.numElements)) {
        x = r;
      }
    } else {
      for (x,r) in (X, chpl__these(startCount, X.numElements)) {
        x = r;
      }
    }
  }

  def fillRandom(x: [] complex) {
    for i in x.domain {
      x(i).re = getNext();
      x(i).im = getNext();
    }
  }

  //
  // The following are internal-only iterators used by the fillRandom
  // routine.  We would like to support general leaders/followers on
  // RandomStreams, but the update to the shared cursor (sharedCount)
  // is not possible to do in a safe way with our current iterator
  // interface.  Future work should help with this.
  // 
  def chpl__these(startAt: int(64), numElements) {
    skipToNth(startAt);
    for i in 1..numElements {
      const val = getNext();
      yield val;
    }
  }

  def chpl__these(param tag: iterator, startAt: int(64), numElements) 
      where tag == iterator.leader {
    var D: domain(1, int(64)) = [1..numElements:int(64)];
    halt("Someone's trying to call the leader");
    yield D;
  }

  def chpl__these(param tag: iterator, follower, startAt: int(64), numElements)
      where tag == iterator.follower {
    // make a local copy of the 'this' random stream class
    var locStream = new RandomStream(seed);
    locStream.skipToNth(follower.low + startAt);
    for i in follower {
      if (debugParRandom) then
        writeln("Doing iteration ", format("#####", i+1), " on locale ", here.id);
      const val = locStream.getNext();
      yield val;
    }
    delete locStream;
  }

  /*  BLC: Would like to add something like this, but should
      really add a warning if the range is greater than max(int(64))
      or a real's precision something to deal with the possible
      accuracy issues...
  def fillRandom(x: [], 
		 minval: x.eltType = min(x.eltType),
		 maxval: x.eltType = max(x.eltType))
               where _isIntegralType(x.eltType) {
    const numVals = maxval:real - minval:real + 1;
    for i in x.domain {
      x(i) = (minval + (getNext() * numVals)): x.eltType;
    }
  }
  */
}


def fillRandom(x:[], initseed: int(64) = SeedGenerator.clockMS) {
  use Random; // This is ridiculous, but only required for modules
              // that call Random.fillRandom without use-ing Random
              // like test/modules/standard/stonea/fillRandom.chpl
  var randNums = new RandomStream(initseed);

  randNums.fillRandom(x); 
}
