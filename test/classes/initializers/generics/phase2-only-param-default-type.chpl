// Verifies the behavior of a Phase 2 only initializer for a param field with
// a default type

class Foo {
  param p: int;

  proc init() {
    this.complete();
    p = 4; // This should fail
  }
}

var foo = new unmanaged Foo();
writeln(foo);
delete foo;
