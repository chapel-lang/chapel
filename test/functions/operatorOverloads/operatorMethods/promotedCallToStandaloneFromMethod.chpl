// This test locks in that we can make a promoted standalone operator call in a
// method on a type that has an operator method for that same operator
record Foo {
  var x: [0..3] Bar;
}
operator Foo.!=(lhs: Foo, rhs: Foo) {
  return lhs.x != rhs.x;
}
proc Foo.myMethod(other: [] Bar) {
  if (& reduce(other != x)) {
    writeln("Replacing");
    x = other;
  } else {
    writeln("Did not replace");
  }
}
record Bar {
  var x: int;
}

operator !=(lhs: Bar, rhs: Bar) {
  return lhs.x != rhs.x;
}

proc main() {
  var foo: Foo;
  foo.x[2] = new Bar(2);
  writeln(foo);
  var arr: [0..3] Bar = [ new Bar(0), new Bar(1), new Bar(2), new Bar(3) ];
  writeln(arr);
  foo.myMethod(arr);
  writeln(foo);
}
