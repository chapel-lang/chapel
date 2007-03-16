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
  const r23   = 0.5**23,
        t23   = 2.0**23,
        r46   = 0.5**46,
        t46   = 2.0**46;

  var cursorVal: real;

  def initialize() {
    cursorVal = seed:real;
    getNext();
  }

  def randlc(inout x) {
    var t1 = r23 * arand;
    const a1 = floor(t1),
          a2 = arand - t23 * a1;
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

  // NOTE: this routine is current untested.
  // reworked things a bit to simplify the
  // interface, take advantage of class
  // ability to retain state, but haven't
  // tried this routine yet
  def getNth(in n : int) {
    var x  = seed:real,
        t = arand;
    while (n != 0) {
      const i = n / 2;
      if (2 * i != n) then
        nextrandlc(x);
      nextrandlc(t);
      n = i;
    }
    return x;
  }


  def nextrandlc() {
    return randlc(cursorVal);
  }

  def getNext() {
    return nextrandlc();
  }

  def fillRandom(x:[?D] real) {
    for i in D {
      x(i) = nextrandlc();
    }
  }

  def fillRandom(x:[?D] complex) {
    for i in D {
      x(i).re = nextrandlc();
      x(i).im = nextrandlc();
    }
  }
}


def fillRandom(x:[], initseed: int(64) = SeedGenerator.clockMS) {
  var randNums = RandomStream(initseed);

  randNums.fillRandom(x); 
}
