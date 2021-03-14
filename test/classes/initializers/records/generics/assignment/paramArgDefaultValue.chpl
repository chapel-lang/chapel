// This test exercises when a generic param argument to an initializer on a
// generic record with a param field has a default value

record Foo {
  param p: int;

  proc init(param pVal = 3) {
    p = pVal;
  }
}

var foo1 = new Foo(1); // specifies a different value
var foo2 = new Foo(); // relies on the default value
var foo3 = new Foo(3); // specifies the same value as the default

writeln(foo1.type:string);
writeln(foo2.type:string);
writeln(foo3.type:string);
