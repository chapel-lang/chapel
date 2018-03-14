// Verifies we currently don't allow initializers to have throw statements in
// their body.
// We hope to eventually allow it.
class Foo {
  var x: int;

  proc init() {
    x = 10;
    throw new Error();
  }
}

var foo = new Foo();
writeln(foo);
delete foo;
