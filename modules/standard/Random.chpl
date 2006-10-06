const
  arand = 1220703125.0,
  FT_seed  = 314159265.0,
  r23   = 0.5**23,
  t23   = 2.0**23,
  r46   = 0.5**46,
  t46   = 2.0**46;

var seed = FT_seed;

def nextrandlc(x : float, a : float) {
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

def initrandlc(seed, a : float, in n : int) : float {
  var i : int, t : float, g : float;
  var x : float = seed;
  t = a;
  while n != 0 do {
    i = n / 2;
    if 2 * i != n then
      (x, g) = nextrandlc(x, t);
    (t, g) = nextrandlc(t, t);
    n = i;
  }
  return x;
}

var randlc_last_n : int = -2,
    randlc_last_x : float;
def randlc(n : int) : float {
  var result : float;
  if n != randlc_last_n + 1 then
    randlc_last_x = initrandlc(seed, arand, n);
  randlc_last_n = n;
  (randlc_last_x, result) = nextrandlc(randlc_last_x, arand);
  return result;
}

def fillRandomVec(x:[] float) {

  [i in x.domain] x(i)= randlc(i); 

}

def fillRandomVec(initseed: float, x: [] float){

  seed = initseed;
  [i in x.domain] x(i)= randlc(i); 

}

