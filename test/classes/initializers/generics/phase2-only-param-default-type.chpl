// Verifies the behavior of a Phase 2 only initializer for a param field with
// a default type

class Foo {
  param p: int;

  proc init() {
    p = 4; // This should fail
  }
}

var foo = new Foo();
writeln(foo);
delete foo;
