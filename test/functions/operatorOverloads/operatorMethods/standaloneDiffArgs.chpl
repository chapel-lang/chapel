record Foo {
  var x: int;
}
operator Foo.+(lhs: Foo, rhs: Foo) {
  writeln("In Foo.+");
  return new Foo(lhs.x + rhs.x);
}
proc +(lhs: Foo, rhs: int) {
  writeln("In standalone +");
  return new Foo(lhs.x + rhs);
}

proc main() {
  var a = new Foo(2);
  var b = new Foo(3);
  var c = a + b;
  writeln(c);
  var d = c + 4;
  writeln(d);
}
