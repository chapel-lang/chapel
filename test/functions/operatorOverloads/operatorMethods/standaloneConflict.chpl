record Foo {
  var x: int;
}
operator Foo.+(lhs: Foo, rhs: Foo) {
  writeln("In Foo.+");
  return new Foo(lhs.x + rhs.x);
}
operator +(lhs: Foo, rhs: Foo) {
  writeln("In standalone +");
  return new Foo(lhs.x + rhs.x);
}

proc main() {
  var a = new Foo(2);
  var b = new Foo(3);
  var c = a + b; // Should trigger conflict
  writeln(c);
}
