// This test is intended for the world where we can generate type constructors
// based on the argument lists of the initializers defined on the type.  Here,
// the generic instantiation would be valid, as the type given matches the type
// of the default expression for the argument to the initializer
class Foo {
  var x;
  var y;

  proc init(xVal = 3) {
    x = xVal;
    y = xVal + 2;
  }
}

var foo: Foo(int);
writeln(foo.type: string);
