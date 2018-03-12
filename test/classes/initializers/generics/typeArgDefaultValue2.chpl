// This test exercises when a generic type argument to an initializer on a
// generic class with a type field has a default value

class Foo {
  type t = bool;
  var x: t;

  proc init(type tVal = bool) {
    t = tVal;
  }
}

var foo1 = new Foo(int); // specifies a different value
var foo2 = new Foo(); // relies on the default value
var foo3 = new Foo(bool); // specifies the same value as the default

writeln(foo1);
writeln(foo2);
writeln(foo3);

delete foo1;
delete foo2;
delete foo3;
