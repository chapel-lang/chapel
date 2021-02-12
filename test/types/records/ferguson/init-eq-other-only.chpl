record InitEqOtherOnly {
  var x: int;
}
proc InitEqOtherOnly.init=(other:int) {
  this.x = other;
}
proc =(ref lhs:InitEqOtherOnly, rhs:InitEqOtherOnly) {
  lhs.x = rhs.x;
}
proc _cast(type t: InitEqOtherOnly, from: int) {
  var tmp: t = from;
  return tmp;
}

proc main() {
  var a: InitEqOtherOnly = 1;
  var acopy = a;
  writeln(a);
  writeln(acopy);
}
