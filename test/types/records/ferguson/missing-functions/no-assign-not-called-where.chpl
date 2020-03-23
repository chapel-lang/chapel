record R {
  var x: int;
}
proc R.init=(other: R) {
  this.x = other.x;
}
// record opts out of being assignable
proc =(ref lhs: R, const ref rhs: R) where false{
}

proc main() {
  var r = new R(1);
  writeln(r);

  {
    use Types;

    assert(isAssignable(R) == false);
    assert(isConstAssignable(R) == false);
  }
}
