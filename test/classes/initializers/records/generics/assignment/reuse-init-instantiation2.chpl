// This test exercises calling the same initializer twice when a record has a
// type field.  In this version, there is an additional argument

record Foo {
  type t;
  var x: t;

  proc init(type tVal, xVal:tVal) {
    t = tVal;
    x = xVal;
  }
}

var foo1 = new Foo(int, 2);
var foo2 = new Foo(int, 4);

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);
