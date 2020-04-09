proc foo(n: int) {
  writeln("in foo, n is: ", n);
  return n;
}

proc main() {
  var a, b: int;

  (a, (_, b)) = (1, (foo(2), foo(3)));
  writeln(a, " ", b);
}
