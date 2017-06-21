// TO REMOVE BEFORE 1.16
class Foo {
  var x: int;

  proc init(xVal) {
    x = xVal;
    super.init();
  }
}

var foo = new Foo(10);
writeln(foo);
delete foo;
