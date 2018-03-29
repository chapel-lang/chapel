// This test exercises calling the same initializer twice when a class has a
// generic var field, but using different types for the generic argument

class Foo {
  var x;

  proc init(xVal) {
    x = xVal;
  }
}

var foo1 = new Foo(1);
var foo2 = new Foo(true);

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);

delete foo1;
delete foo2;
