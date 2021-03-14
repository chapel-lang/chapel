// Verifies we currently don't allow initializers to have try statements in
// their body.
// We hope to eventually allow it.
class Foo {
  var x: int;

  proc init() {
    x = 10;
    try {
      outerFunc();
    }
  }
}

proc outerFunc() throws {
  throw new owned Error();
}

var foo = new unmanaged Foo();
writeln(foo);
delete foo;
