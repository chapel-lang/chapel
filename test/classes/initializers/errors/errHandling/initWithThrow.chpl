// Verifies we currently don't allow initializers to have throw statements in
// their body.
// We hope to eventually allow it.
class Foo {
  var x: int;

  proc init() {
    x = 10;
    throw new owned Error();
  }
}

var foo = new owned Foo();
writeln(foo);
