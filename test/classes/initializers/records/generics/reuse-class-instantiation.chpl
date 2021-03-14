// This test exercises calling two initializers that lead to the same
// instantiation when a record has a type field

record Foo {
  type t;
  var x: t;

  proc init(type t) {
    this.t = t;
  }

  proc init(xVal) where !isSubtype(xVal.type, Foo) {
    t = xVal.type;
    x = xVal;
  }
}

var foo1: Foo(int);
var foo2 = new Foo(3);

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);
