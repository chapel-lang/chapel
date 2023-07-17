// This test exercises calling the same initializer twice when a class has a
// type field.  In this version, there is an additional argument

class Foo {
  type t;
  var x: t;

  proc init(type tVal, xVal:tVal) {
    t = tVal;
    x = xVal;
  }
}

var ownFoo1 = new owned Foo(int, 2);
var foo1 = ownFoo1.borrow();
var ownFoo2 = new owned Foo(int, 4);
var foo2 = ownFoo2.borrow();

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);
