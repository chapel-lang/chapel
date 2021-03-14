// This test exercises when a generic param argument to an initializer on a
// generic record with a param field has a default value

record Foo {
  param p: int;

  proc init(param p = 3) {
    this.p = p;
  }
}

var foo1: Foo(1); // specifies a different value
var foo3: Foo(3); // specifies the same value as the default

writeln(foo1.type:string);
writeln(foo3.type:string);
