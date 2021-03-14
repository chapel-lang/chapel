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
  return true;
}

var myR  = new R(1);
var myR2 = new R(2);

myR = myR2;
writeln("myR is: ", myR);

