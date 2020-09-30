// assignopLHSargIsRef.chpl
//
// Tests the check that the LHS argument of an assignment operator override is
// passed by ref.

config param test = 0;

record R {
  var x: int;
}

proc =(a:R, b:R) {
  a.x = b.x;
}

proc *=(a:R, b:R) {
  a.x += b.x;
}

var r1 = new R(1);
var r2 = new R(2);

r1 = r2;
r1 *= r2;
