// This test exercises calling the same initializer twice when a class has a
// param field

class Foo {
  param p: int;
  var x: int;

  proc init(param pVal) {
    p = pVal;
  }
}

var ownFoo1 = new owned Foo(2);
var foo1 = ownFoo1.borrow();
var ownFoo2 = new owned Foo(2);
var foo2 = ownFoo2.borrow();

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);
