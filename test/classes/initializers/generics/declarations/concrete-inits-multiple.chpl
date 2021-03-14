// This test exercises more of creating a type constructor from the initializer
// argument list.  If and when we support that, this is intended to valid the
// behavior of initializers with concrete arguments.
class Foo {
  var x;
  var y;
  var z;

  proc init(xVal: int, yVal: real) {
    x = xVal;
    y = yVal;
    z = xVal + 2;
  }
}

var foo1: Foo(int, real);
writeln(foo1.type: string);
