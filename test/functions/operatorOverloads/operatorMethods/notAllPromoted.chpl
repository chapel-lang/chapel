record Foo {
  var x: [0..3] int;
}
operator Foo.+(lhs: Foo, rhs: Foo) {
  return new Foo(lhs.x + rhs.x);
}

proc Foo.someMethod(scalarAdd: int) {
  x = x + scalarAdd;
}

proc main() {
  var foo: Foo = new Foo([1, 3, 5, 7]);
  writeln(foo);
  foo.someMethod(3);
  writeln(foo);
}
