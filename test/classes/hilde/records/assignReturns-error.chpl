// Shows that returning a value from an assignment elicits the error check.
// in checkResolved(): The return value of an assignment operator must be
// 'void'. 

record R {
  var x: int;
}

proc R.init=(const ref other:R) {
  this.x = other.x;
}

proc =(ref lhs:R, rhs: R) {
  lhs.x = rhs.x + 1;
  return new R();
}

var myR, myR2: R;

myR = myR2;
writeln("myR is: ", myR);

