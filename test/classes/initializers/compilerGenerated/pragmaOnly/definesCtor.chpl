// Verifies behavior when type defines an explicit constructor
pragma "use default init"
class Foo {
  var x: int;

  proc Foo() {
    x = 10;
  }
}

var x = new Foo();
writeln(x);
delete x;
