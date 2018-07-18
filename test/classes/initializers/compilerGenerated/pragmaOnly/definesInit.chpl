// Verifies behavior when type defines an explicit initializer
class Foo {
  var x: int;

  proc init() {
    x = 10;
  }
}

var x = new Foo();
writeln(x);
delete x;
