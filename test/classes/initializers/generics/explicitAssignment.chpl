// Verifies the behavior of generic types with initializers when the expected
// type differs from what type would be generated from the initializer only
class Foo {
  var x;

  proc init(xVal: int) {
    x = xVal;

  }

  proc init(xVal: real) {
    x = xVal;

  }
}

proc main() {
  var a: borrowed Foo(real) = new borrowed Foo(1);
  var b = new borrowed Foo(1.0);
  writeln(a);
  writeln(b);
}
