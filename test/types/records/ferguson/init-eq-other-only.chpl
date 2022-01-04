record InitEqOtherOnly {
  var x: int;
}
proc InitEqOtherOnly.init=(other:int) {
  this.x = other;
}
operator InitEqOtherOnly.=(ref lhs:InitEqOtherOnly, rhs:InitEqOtherOnly) {
  lhs.x = rhs.x;
}
operator :(from: int, type t: InitEqOtherOnly) {
  var tmp: t = from;
  return tmp;
}

proc main() {
  var a: InitEqOtherOnly = 1;
  var acopy = a;
  writeln(a);
  writeln(acopy);
}
