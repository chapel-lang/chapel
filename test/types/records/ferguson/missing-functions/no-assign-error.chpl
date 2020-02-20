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

  var r2 = new R(2);
  r2 = r;
}
