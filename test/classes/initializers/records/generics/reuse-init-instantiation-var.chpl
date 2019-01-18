// This test exercises calling the same initializer twice with the same
// arguments when a record has a generic var field

record Foo {
  var x;

  proc init(x) where !isSubtype(x.type, Foo) {
    this.x = x;
  }
}

var foo1: Foo(int);
var foo2: Foo(int);

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);
