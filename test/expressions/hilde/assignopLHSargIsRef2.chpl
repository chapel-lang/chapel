// assignopLHSargIsRef.chpl
//
// Tests the check that the LHS argument of an assignment operator override is
// passed by ref.

config param test = 0;

record R {
  var x: int;
}

proc R.init=(other:R) {
  this.x = other.x;
}
operator =(a:R, b:R) {
  writeln("in R's = operator");
}

operator R.*=(a:R, b:R) {
  writeln("in R's *= operator");
}

var r1 = new R(42),
    r2 = new R(23);

r1 = r2;
r1 *= r2;

writeln(r1, r2);
