// This test exercises using a type alias as the type of a field we set in the
// body of the initializer.
type tAlias = bool;

class Foo {
  var x: tAlias;

  proc init(xVal: tAlias) {
    x = xVal;
  }
}

var foo = new owned Foo(true);
writeln(foo);
