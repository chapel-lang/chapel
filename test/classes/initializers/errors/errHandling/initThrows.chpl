// Verifies we allow initializers to be declared as throws, regardless of what
// they contain in their body
class Foo {
  var x: int;

  proc init() throws {
    x = 10;
  }
}

var foo = new unmanaged Foo();
writeln(foo);
delete foo;
