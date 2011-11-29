proc mymin(type t): t {
  return -999;
}

var minint: int = mymin(int);

writeln("minint is: ", minint);
