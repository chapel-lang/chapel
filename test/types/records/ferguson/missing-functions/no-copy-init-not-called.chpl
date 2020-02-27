record R {
  var x: int;
}
// record opts out of being copyable
proc R.init=(other: R) {
  compilerError("You can't copy an R");
}
proc =(ref lhs: R, const ref rhs: R) {
  lhs.x = rhs.x;
}

// program does not try to copy initialize R
proc main() {
  var r = new R(1);
  writeln(r);

  {
    use Types;

    assert(isCopyable(R) == false);
    assert(isConstCopyable(R) == false);
  }
}
