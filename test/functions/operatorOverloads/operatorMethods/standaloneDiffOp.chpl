record Foo {
  var x: int;
}
operator Foo.+(lhs: Foo, rhs: Foo) {
  writeln("In Foo.+");
  return new Foo(lhs.x + rhs.x);
}
proc -(lhs: Foo, rhs: Foo) {
  writeln("In standalone -");
  return new Foo(lhs.x - rhs.x);
}

proc main() {
  var a = new Foo(2);
  var b = new Foo(3);
  var c = a + b;
  writeln(c);
  var d = new Foo(1);
  var e = c - d;
  writeln(e);
}
