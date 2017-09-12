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
    super.init();
  }
}

proc outerFunc() throws {
  throw new Error();
}

var foo = new Foo();
writeln(foo);
delete foo;
