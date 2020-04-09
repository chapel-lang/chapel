// This test exercises calling the same initializer twice when a record has a
// type field

record Foo {
  type t;
  var x: t;

  proc init(type tVal) {
    t = tVal;
  }
}

var foo1: Foo(int) = new Foo(int);
var foo2: Foo(int) = new Foo(int);

writeln(foo1.type == foo2.type);
