record Foo {
  var x: int;

  operator +(lhs: Foo, rhs: Foo) {
    return new Foo(lhs.x + rhs.x);
  }
}

proc main() {
  var first = new Foo(3);
  var second = new Foo(2);
  var third = first + second;
  writeln(third);
}
