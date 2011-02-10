proc mymin(type t) {
  var x: t;
  x = -999;
  return x;
}

var minint: int = mymin(int);

writeln("minint is: ", minint);
