// Verifies the behavior of a Phase 2 only initializer for a param field with
// a default value

class Foo {
  param p = 3;

  proc init() {
    init this;
    p = 4; // This should fail
  }
}

var foo = new shared Foo();
writeln(foo);
