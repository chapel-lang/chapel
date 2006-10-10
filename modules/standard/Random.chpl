const
  FT_arand = 1220703125.0,
  FT_seed  = 314159265.0;

class rng {

  var seed = FT_seed;
  var arand = FT_arand;

  const
    r23   = 0.5**23,
    t23   = 2.0**23,
    r46   = 0.5**46,
    t46   = 2.0**46;

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

  def fillRandom(x:[?D] float) {

    if (D.rank > 1) {
      writeln ("Not yet implemented for 2D or higher arrays.");
      [i in D] x(i) = -1.0;
    }
    else if (D.rank == 1) {
      var n = D(1).length;
      var innerD: domain(1);
      if (n > 128) {
        for i in D by 128 {
          var randlc_last_x: float = initrandlc(seed, arand, i);
          if (i+127 > n) then
            innerD = [i..n];
          else 
            innerD = [i..i+127];
          for j in innerD {
              (randlc_last_x,x(j)) = nextrandlc(randlc_last_x, arand); 
          }
        }
      }
      else {
        var randlc_last_x: float = initrandlc(seed, arand, 1);
        x(1) = randlc_last_x;
        for i in [2..n] {
          (randlc_last_x,x(i)) = nextrandlc(randlc_last_x, arand);    
        }
      }
    seed = x(n);
    }
  }
}
