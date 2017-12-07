// Verifies behavior when type defines multiple explicit constructors
pragma "use default init"
class Foo {
  var x: int;

  proc Foo() {
    x = 10;
  }

  proc Foo(xVal) {
    x = xVal;
  }
}

var x = new Foo();
writeln(x);
delete x;
