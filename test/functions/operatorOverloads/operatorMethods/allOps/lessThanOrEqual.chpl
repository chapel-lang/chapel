// Tests defining <= as an operator method
record Foo {
  var x: int;

  operator <=(lhs: Foo, rhs: Foo) {
    writeln("In Foo.<=");
    return(lhs.x <= rhs.x);
  }
}

proc main() {
  var a = new Foo(3);
  var b: Foo;
  writeln(a);
  writeln(b);
  writeln(a <= b);
  writeln(a <= a);
  var c = new Foo(3);
  writeln(a <= c);
  var d = new Foo(7);
  writeln(a <= d);
}
