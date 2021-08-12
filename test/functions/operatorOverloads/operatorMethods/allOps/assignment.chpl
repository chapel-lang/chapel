// Tests defining = as an operator method
record Foo {
  var x: int;

  proc init=(other: Foo) {
    writeln("In init=");
    this.x = other.x;
  }

  operator =(ref lhs: Foo, rhs: Foo) {
    writeln("In Foo.=");
    lhs.x = rhs.x;
  }
}

proc main() {
  var a = new Foo(3);
  var b: Foo;
  writeln(a);
  writeln(b);
  b = a;
  writeln(b);
}
