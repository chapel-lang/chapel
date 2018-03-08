// Verifies the behavior of a Phase 2 only initializer for a type field with
// a default value

class Foo {
  type t = int;

  proc init() {
    this.initDone();
    t = int; // This should fail
  }
}

var foo = new Foo();
writeln(foo);
delete foo;
