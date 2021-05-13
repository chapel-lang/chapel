record R {
  var f: int;
}

proc someFunc(a: R, b: R) {
  operator R.+(lhs: R, rhs: R) {
    writeln("In nested operator method");
    return new R(lhs.f + rhs.f);
  }
  var c = a + b;
  return c;
}

proc main() {
  var r1 = new R(3);
  var r2: R = new R(1);
  var r3 = someFunc(r1, r2);
  writeln(r3);
  var r4 = r1 + r3; // Should fail, the overload is not defined here
  writeln(r4);
}
