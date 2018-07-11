// Verifies we currently allow initializers to have try! statements in their
// body, as long as it doesn't have a catch block.
class Foo {
  var x: int;

  proc init() {
    x = 10;
    try! outerFunc();

  }
}

proc outerFunc() throws {
  throw new Error();
}

var foo = new Foo();
writeln(foo);
delete foo;
