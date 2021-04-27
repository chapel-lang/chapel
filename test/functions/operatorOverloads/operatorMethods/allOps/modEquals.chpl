// Test binary %= operator as an operator method
record Foo {
  var x: int;

  operator %=(ref lhs: Foo, rhs: Foo) {
    writeln("In Foo.%=");
    lhs.x %= rhs.x;
  }
}

proc main() {
  var a = new Foo(6);
  var b = new Foo(3);
  writeln(a);
  writeln(b);
  a %= b;
  writeln(a);
}
