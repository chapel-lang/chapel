// This test exercises calling the same initializer twice when a record has a
// param field

record Foo {
  param p: int;
  var x: int;

  proc init(param pVal) {
    p = pVal;
  }
}

var foo1: Foo(2) = new Foo(2);
var foo2: Foo(2) = new Foo(2);

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);
