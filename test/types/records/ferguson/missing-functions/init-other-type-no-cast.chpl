record R {
  var x: int;
}
// same-type = and init=
proc =(ref lhs: R, const ref rhs: R) {
  lhs.x = rhs.x;
}
proc R.init=(other: R) {
  this.x = other.x;
}

// cross-type init= and =
/*proc =(ref lhs: R, rhs: int) {
  lhs.x = rhs;
}*/
proc R.init=(other: int) {
  this.x = other;
}
/*proc _cast(type t: R, x: int) {
  return new R(x);
}*/

// program tries to copy-init R
proc ret(arg) { return arg; }

proc main() {
  var r:R = 1;
  writeln(r);
}
