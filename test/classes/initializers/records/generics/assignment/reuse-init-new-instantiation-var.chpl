// This test exercises calling the same initializer twice when a record has a
// generic var field, but using different types for the generic argument

record Foo {
  var x;

  proc init(xVal) where !isSubtype(xVal.type, Foo) {
    x = xVal;
  }
}

var foo1 = new Foo(1);
var foo2 = new Foo(true);

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);
