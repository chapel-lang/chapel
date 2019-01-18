// This test exercises when a generic value argument to an initializer on a
// generic record with a generic var field has a default value

record Foo {
  var v;

  proc init(vVal = 3) {
    v = vVal;
  }
}

var foo1 = new Foo(true); // specifies a different value (of a different type)
var foo2 = new Foo(); // relies on the default value
var foo3 = new Foo(3); // specifies the same value as the default

writeln(foo1.type:string);
// Since the only initializer for this type takes in an integer, the type of
// foo1 will get coerced to Foo(int)
writeln(foo1);
writeln(foo2);
writeln(foo3);
