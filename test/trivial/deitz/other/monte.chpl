config var n: int = 100000;

iter randomPoints(n: int, seed: uint = 31415926) {
  proc next(inout x: uint) {
    var multiplier = 16807, modulus = 2147483647;
    x = (x:int(64) * multiplier % modulus): uint;
    return x:real / modulus;
  }

  var x = seed;
  for i in 1..n {
    yield (next(x), next(x));
  }
}

proc d2((x,y)) return x**2 + y**2;

writeln((+ reduce (d2(randomPoints(n)) < 1.0)) * 4 / n:real);
