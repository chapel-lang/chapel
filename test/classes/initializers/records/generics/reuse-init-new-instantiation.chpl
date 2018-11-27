// This test exercises calling the same initializer twice when a record has a
// type field, but using different values for the generic argument

record Foo {
  type t;
  var x: t;

  proc init(type t) {
    this.t = t;
  }
}

var foo1: Foo(int);
var foo2: Foo(bool);

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);
