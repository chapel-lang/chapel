record Foo {
  var x: int;

  deprecated "Addition of Foos is deprecated"
  operator +(arg1: Foo, arg2: Foo) {
    return new Foo(arg1.x + arg2.x);
  }
}

deprecated "Subtraction of Foos is deprecated"
operator Foo.-(arg1: Foo, arg2: Foo) {
  return new Foo(arg1.x - arg2.x);
}

proc main() {
  var a = new Foo(3);
  var b = new Foo(2);
  writeln(a + b);
  writeln(a - b);
}
