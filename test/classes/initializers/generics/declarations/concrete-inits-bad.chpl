// This test is intended for the world where we can generate type constructors
// from initializers, to verify that certain failure modes exist - in this case,
// the failure mode is that we won't be able to take "imag" as a valid value for
// the second argument to the type constructor
class Foo {
  var x;
  var y;

  proc init(xVal: int) {
    x = xVal;
    y = xVal + 2;
  }

  proc init(xVal: real) {
    x = xVal;
    y = xVal + 2;
  }
}

var foo1: Foo(imag);
writeln(foo1.type: string);
