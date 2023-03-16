// This test exercises calling the same initializer twice when a class has a
// type field, but using different values for the generic argument

class Foo {
  type t;
  var x: t;

  proc init(type tVal) {
    t = tVal;
  }
}

var ownFoo1 = new owned Foo(int);
var foo1 = ownFoo1.borrow();
var ownFoo2 = new owned Foo(bool);
var foo2 = ownFoo2.borrow();

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);
