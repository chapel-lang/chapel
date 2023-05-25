// This test exercises when a generic type argument to an initializer on a
// generic class with a type field has a default value

class Foo {
  type t;
  var x: t;

  proc init(type tVal = bool) {
    t = tVal;
  }
}

var foo1 = (new owned Foo(int)).borrow(); // specifies a different value
var foo2 = (new owned Foo()).borrow(); // relies on the default value
var foo3 = (new owned Foo(bool)).borrow(); // specifies the same value as the default

writeln(foo1);
writeln(foo2);
writeln(foo3);
