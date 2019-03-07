// Verifies that my insertion of omitted initialization works fine when the
// type of the field requires a cast, or is a specific size of integer instead
// of the default
class Foo {
  var v = 4:uint; // cast
  var x: real = 5; // coercion
  var y: int(32); // non-default int
  var z: int = 15; // basic field, verified by other tests.

  proc init(zVal: int) {
    z = zVal;
  }
}

var a: borrowed Foo = new borrowed Foo(7);
writeln(a);
