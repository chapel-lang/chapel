record Foo {
  var x: int;
}
proc +(lhs: Foo, rhs: Foo) {
  return new Foo(lhs.x + rhs.x);
}

proc -(lhs: Foo, rhs: Foo) {
  return new Foo(lhs.x - rhs.x);
}

proc main() {
  var a = new Foo(2);
  var b = a + a;
  writeln(b);
  var c = new Foo(1);
  writeln(b - c);
}
