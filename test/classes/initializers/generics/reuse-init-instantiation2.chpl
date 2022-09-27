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

var foo1 = (new owned Foo(int, 2)).borrow();
var foo2 = (new owned Foo(int, 4)).borrow();

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);
