// This test exercises when a generic value argument to an initializer on a
// generic record with a generic var field has a default value

record Foo {
  var v;

  proc init(v = 3) {
    this.v = v;
  }
}

var foo1: Foo(bool); // specifies a different value (of a different type)
var foo2: Foo(int); // relies on the default value

writeln(foo1.type:string);
// Since the only initializer for this type takes in an integer, the type of
// foo1 will get coerced to Foo(int)
writeln(foo1);
writeln(foo2);
