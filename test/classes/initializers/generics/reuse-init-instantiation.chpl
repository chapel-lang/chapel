// This test exercises calling the same initializer twice when a class has a
// type field

class Foo {
  type t;
  var x: t;

  proc init(type tVal) {
    t = tVal;
  }
}

var foo1 = (new owned Foo(int)).borrow();
var foo2 = (new owned Foo(int)).borrow();

writeln(foo1.type == foo2.type);
