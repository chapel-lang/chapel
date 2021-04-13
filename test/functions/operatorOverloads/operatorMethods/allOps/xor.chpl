// Test ^ operator as an operator method
record Foo {
  var x: bool;

  operator ^(lhs: Foo, rhs: Foo) {
    writeln("In Foo.^");
    return new Foo(lhs.x ^ rhs.x);
  }
}

proc main() {
  var a = new Foo(true);
  var b = new Foo(false);
  writeln(a ^ a);
  writeln(a ^ b);
  writeln(b ^ b);
  var c = new Foo(true);
  var d = new Foo(false);
  writeln(a ^ c);
  writeln(b ^ d);
}
