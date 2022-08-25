// This test exercises calling the same initializer twice when a class has a
// param field

class Foo {
  param p: int;
  var x: int;

  proc init(param pVal) {
    p = pVal;
  }
}

var foo1 = (new owned Foo(2)).borrow();
var foo2 = (new owned Foo(2)).borrow();

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);
