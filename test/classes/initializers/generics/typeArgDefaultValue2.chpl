// This test exercises when a generic type argument to an initializer on a
// generic class with a type field has a default value

class Foo {
  type t = bool;
  var x: t;

  proc init(type tVal = bool) {
    t = tVal;
  }
}

var ownFoo1 = new owned Foo(int);  // specifies a different value
var foo1 = ownFoo1.borrow();
var ownFoo2 = new owned Foo();     // relies on the default value
var foo2 = ownFoo2.borrow();
var ownFoo3 = new owned Foo(bool); // specifies the same value as the default
var foo3 = ownFoo3.borrow();

writeln(foo1);
writeln(foo2);
writeln(foo3);
