class Foo {
  var x = xTimes3();
  var y = 5;

  proc init(yVal) {
    y = yVal;
  }

  proc xTimes3() {
    return x * 3;
  }
}
var foo = new Foo(2);
writeln(foo);
delete foo;
