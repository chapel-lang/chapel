// This test exercises when a generic type argument to an initializer on a
// generic record with a type field has a default value

record Foo {
  type t;
  var x: t;

  proc init(type tVal = bool) {
    t = tVal;
  }
}

var foo1: Foo(int) = new Foo(int); // specifies a different value
var foo2: Foo(bool) = new Foo(); // relies on the default value
var foo3: Foo(bool) = new Foo(bool); // specifies the same value as the default

writeln(foo1);
writeln(foo2);
writeln(foo3);
