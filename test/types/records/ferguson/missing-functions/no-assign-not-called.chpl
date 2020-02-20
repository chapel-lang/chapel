record R {
  var x: int;
}

// record opts out of being copyable
proc =(ref lhs: R, const ref rhs: R) {
  compilerError("You can't assign an R");
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
