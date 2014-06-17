proc mymin(type t) {
  return -999;
}

var minint: int = mymin(int);
var minreal: real = mymin(real);

writeln("minint is: ", minint);
writeln("minreal is: ", minreal);

