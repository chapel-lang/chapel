private proc foo(x: int) {
  // Should not be visible outside of definesPrivate
  var res = 14 + x;
  return res;
}

proc bar(y: int) {
  // Should still be visible when module is used.
  var res = 20 - y;
  return res;
}

proc main() {
  writeln(foo(2));
  writeln(bar(2));
}

proc printFoo() {
  writeln(foo(2));
}
