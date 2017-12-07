// Verifies behavior when type defines an explicit initializer
pragma "use default init"
class Foo {
  var x: int;

  proc init() {
    x = 10;
  }
}

var x = new Foo();
writeln(x);
delete x;
