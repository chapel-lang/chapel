// This test exercises calling the same initializer twice when a record has a
// type field, but using different values for the generic argument

record Foo {
  type t;
  var x: t;

  proc init(type tVal) {
    t = tVal;
  }
}

var foo1 = new Foo(int);
var foo2 = new Foo(bool);

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);
