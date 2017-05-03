class Foo {
  var x;
  var y;
  var z;

  proc init(xVal: int, yVal: real) {
    x = xVal;
    y = yVal;
    z = xVal + 2;
    super.init();
  }
}

var foo1: Foo(int, real);
writeln(foo1.type: string);
