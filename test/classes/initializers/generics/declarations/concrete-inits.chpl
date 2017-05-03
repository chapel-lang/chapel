class Foo {
  var x;
  var y;

  proc init(xVal: int) {
    x = xVal;
    y = xVal + 2;
    super.init();
  }

  proc init(xVal: real) {
    x = xVal;
    y = xVal + 2;
    super.init();
  }
}

var foo1: Foo(int);
var foo2: Foo(real);
writeln(foo1.type: string);
writeln(foo2.type: string);
