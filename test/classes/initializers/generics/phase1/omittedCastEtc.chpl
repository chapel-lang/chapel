// Verifies that my insertion of omitted initialization works fine when the
// type of the field requires a cast, or is a specific size of integer instead
// of the default
class Foo {
  param v = 4:uint; // cast
  param x: real = 5; // coercion
  param y: int(32); // non-default int
  param z: int = 15; // basic field, verified by other tests.

  proc init(param zVal: int) {
    z = zVal;
  }
}

var a: Foo(4:uint, 5.0, 0:int(32), 7) = new Foo(7);
writeln(a.type: string);
delete a;
