// This test exercises more of creating a type constructor from the initializer
// argument list.  If and when we support that, this is intended to valid the
// behavior of initializers with concrete arguments.
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

var foo1: Foo(int);
var foo2: Foo(real);
writeln(foo1.type: string);
writeln(foo2.type: string);
