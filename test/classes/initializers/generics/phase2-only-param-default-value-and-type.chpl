// Verifies the behavior of a Phase 2 only initializer for a param field with
// a default value and type

class Foo {
  param p: int = 3;

  proc init() {
    this.complete();
    p = 4; // This should fail
  }
}

var foo = new borrowed Foo();
writeln(foo);
