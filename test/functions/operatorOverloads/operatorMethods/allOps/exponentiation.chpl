// Test ** operator as an operator method
record Foo {
  var x: int;

  operator **(lhs: Foo, rhs: Foo) {
    writeln("In Foo.**");
    return new Foo(lhs.x ** rhs.x);
  }
}

proc main() {
  var a = new Foo(6);
  var b = new Foo(2);
  writeln(a ** b);
  var c = new Foo(5);
  writeln(a ** c);
}
