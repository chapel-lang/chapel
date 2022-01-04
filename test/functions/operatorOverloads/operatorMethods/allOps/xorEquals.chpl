// Test binary ^= operator as an operator method
record Foo {
  var x: bool;

  operator ^=(ref lhs: Foo, rhs: Foo) {
    writeln("In Foo.^=");
    lhs.x ^= rhs.x;
  }
}

proc main() {
  var a = new Foo(true);
  var b = new Foo(false);
  writeln(a);
  writeln(b);
  a ^= b;
  writeln(a);
}
