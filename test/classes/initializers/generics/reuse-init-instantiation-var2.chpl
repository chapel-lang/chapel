// This test exercises calling the same initializer twice with different
// arguments of the same type when a class has a generic var field

class Foo {
  var x;

  proc init(xVal) {
    x = xVal;
  }
}

var foo1 = (new owned Foo(1)).borrow();
var foo2 = (new owned Foo(2)).borrow();

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);
