proc mymin(type t): t {
  return -999:t;
}

var minint: int = mymin(int);
var minreal: real = mymin(real);

writeln("minint is: ", minint);
writeln("minreal is: ", minreal);
