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


class SeedGeneratorClass {
  def clockMS {
    return getCurrentTime(unit=microseconds):int(64);
  }
}

var SeedGenerator = SeedGeneratorClass();

class RandomStream {
  const seed:int(64) = SeedGenerator.clockMS; 
  const arand = 1220703125.0;

  // These are meant to be private; internal to class; not set by user
  const r23   = 0.5**23,
        t23   = 2.0**23,
        r46   = 0.5**46,
        t46   = 2.0**46;
  var internalSeed:int(64);

  var cursorVal: real;

  def initialize() {
    internalSeed = seed;
    // ensure seed is odd
    if (internalSeed % 2 == 0) then internalSeed += 1;
    initCursorVal();
  }

  def initCursorVal() {
    cursorVal = internalSeed:real;
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
    return randlc(cursorVal);
  }

  // n is assumed to be 1..
  def getNth(in n : int) {
    n -= 1;
    var t = arand;
    initCursorVal();
    var retval = arand;
    while (n != 0) {
      const i = n / 2;
      if (2 * i != n) then
        randlc(cursorVal, t);
      retval = randlc(t, t);
      n = i;
    }
    return getNext();
  }


  def fillRandom(x: [] real) {
    for i in x.domain {
      x(i) = getNext();
    }
  }

  def fillRandom(x: [] complex) {
    for i in x.domain {
      x(i).re = getNext();
      x(i).im = getNext();
    }
  }
}


def fillRandom(x:[], initseed: int(64) = SeedGenerator.clockMS) {
  var randNums = RandomStream(initseed);

  randNums.fillRandom(x); 
}
