// This test exercises calling the same initializer twice with different
// arguments of the same type when a record has a generic var field

record Foo {
  var x;

  proc init(xVal) where !isSubtype(xVal.type, Foo) {
    x = xVal;
  }
}

var foo1 = new Foo(1);
var foo2 = new Foo(2);

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);
