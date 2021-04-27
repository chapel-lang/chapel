// Test of the cast operator as a method
record Foo {
  var x: int;
}

record Bar {
  var y: int;
}

operator Foo.:(rhs: Bar, type t: Foo) {
  writeln("In Foo.:");
  return new Foo(rhs.y);
}
operator Bar.:(rhs: Foo, type t: Bar) {
  writeln("In Bar.:");
  return new Bar(rhs.x);
}

proc main() {
  var foo1 = new Foo(3);
  var bar1 = new Bar(7);
  var foo2 = bar1: Foo;
  var bar2 = foo1: Bar;
  writeln(foo2);
  writeln(foo2.type : string);
  writeln(bar2);
  writeln(bar2.type : string);
}
