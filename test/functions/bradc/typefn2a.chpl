fun min(type t): t {
  return -999:t;
}

var minint: int = min(int);
var minfloat: float = min(float);

writeln("minint is: ", minint);
writeln("minfloat is: ", minfloat);
