// This test exercises calling the same initializer twice when a record has a
// generic var field, but using different types for the generic argument

record Foo {
  var x;

  proc init(xVal) where (!xVal: Foo) {
    x = xVal;
  }
}

var foo1: Foo(int);
var foo2: Foo(bool);

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);
