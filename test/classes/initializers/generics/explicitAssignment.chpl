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
  var aObj = new Foo(1);   var a: borrowed Foo(real) = aObj.borrow();
  var bObj = new Foo(1.0); var b = bObj.borrow();
  writeln(a.borrow());
  writeln(b.borrow());
}
