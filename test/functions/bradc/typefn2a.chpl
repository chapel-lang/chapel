proc mymymin(type t): t {
  return -999:t;
}

var minint: int = mymymin(int);
var minreal: real = mymymin(real);

writeln("minint is: ", minint);
writeln("minreal is: ", minreal);
