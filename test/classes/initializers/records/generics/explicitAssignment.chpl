// Verifies the behavior of generic types with initializers when the expected
// type differs from what type would be generated from the initializer only
record Foo {
  var x;

  proc init(x: int) {
    this.x = x;
  }

  proc init(x: real) {
    this.x = x;
  }
}

proc main() {
  var a: Foo(real) = new Foo(1);
  var b = new Foo(1.0);
  writeln(a);
  writeln(b);
}
