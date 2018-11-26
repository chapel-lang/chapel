// This test exercises when a generic type argument to an initializer on a
// generic record with a type field has a default value

record Foo {
  type t;
  var x: t;

  proc init(type t = bool) {
    this.t = t;
  }
}

var foo1: Foo(int); // specifies a different value
var foo3: Foo(bool); // specifies the same value as the default

writeln(foo1);
writeln(foo3);
