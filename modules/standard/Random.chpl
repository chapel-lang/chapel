use Time;


class SeedGeneratorClass {
  // want this to be a param, but it doesn't work
  var clockMS = -1;
}

var SeedGenerator = SeedGeneratorClass();

class RandomStream {
  const seed:int(64) = SeedGenerator.clockMS; 
  const arand = 1220703125.0;

  def initialize() {
    if (seed == SeedGenerator.clockMS) then
      seed = getCurrentTime(unit=microseconds):int(64);
    if (seed % 2 == 1) then
      seed += 1;
  }


  var numGenerated = 0;

  def nextrandlc(x : float, a: float = arand) {
    const
      r23   = 0.5**23,
      t23   = 2.0**23,
      r46   = 0.5**46,
      t46   = 2.0**46;
    var t1 = r23 * a;
    var a1 = floor(t1);
    var a2 = a - t23 * a1;
    t1 = r23 * x;
    var x1 = floor(t1);
    var x2 = x - t23 * x1;
    t1 = a1 * x2 + a2 * x1;
    var t2 = floor(r23 * t1);
    var z  = t1 - t23 * t2;
    var t3 = t23 * z + a2 * x2;
    var t4 = floor(r46 * t3);
    var x3 = t3 - t46 * t4;
    return (x3, r46 * x3);
  }

  def initrandlc(in n : int) : float {
    var i : int, t : float, g : float;
    var x  = seed:float;
    t = arand;
    while n != 0 do {
      i = n / 2;
      if 2 * i != n then
        (x, g) = nextrandlc(x, t);
      (t, g) = nextrandlc(t, t);
      n = i;
    }
    return x;
  }

  def fillRandom(x:[?D] float) {

    if (D.rank > 1) {
      writeln ("Not yet implemented for 2D or higher arrays.");
      [i in D] x(i) = -1.0;
    }
    else {
      var randlc_last_x: float = initrandlc(numGenerated+1);
      for i in D {
        (randlc_last_x,x(i)) = nextrandlc(randlc_last_x);    
        numGenerated += 1;
      }
    }
  }

  def fillRandom(x:[?D] complex) {

    if (D.rank > 1) {
      writeln ("Not yet implemented for 2D or higher arrays.");
      [i in D] x(i) = -1.0;
    }
    else {
      var randlc_last_x: float = initrandlc(numGenerated+1);
      for i in D {
        (randlc_last_x,x(i).real) = nextrandlc(randlc_last_x);    
        (randlc_last_x,x(i).imag) = nextrandlc(randlc_last_x);    
        numGenerated += 2;
      }
    }
  }
}


def fillRandom(x:[], initseed: int(64) = SeedGenerator.clockMS) {
  var randNums = RandomStream(initseed);

  randNums.fillRandom(x); 
}
