// Test defining the swap operator (<=>) as an operator method
record Foo {
  var x: int;

  operator <=>(ref lhs: Foo, ref rhs: Foo) {
    writeln("In Foo.<=>");
    var tmp = lhs;
    lhs = rhs;
    rhs = tmp;
  }
}

proc main() {
  var a = new Foo(5);
  var b = new Foo(3);
  writeln(a);
  writeln(b);
  a <=> b;
  writeln(a);
  writeln(b);
}
