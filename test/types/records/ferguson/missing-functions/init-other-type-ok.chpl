record R {
  var x: int;
}
// same-type = and init=
operator R.=(ref lhs: R, const ref rhs: R) {
  lhs.x = rhs.x;
}
proc R.init=(other: R) {
  this.x = other.x;
}

// cross-type init= and =
/*operator =(ref lhs: R, rhs: int) {
  lhs.x = rhs;
}*/
proc R.init=(other: int) {
  this.x = other;
}
operator :(x: int, type t: R) {
  return new R(x);
}

// program tries to copy-init R
proc ret(arg) { return arg; }

proc main() {
  var r:R = 1;
  writeln(r);
}
