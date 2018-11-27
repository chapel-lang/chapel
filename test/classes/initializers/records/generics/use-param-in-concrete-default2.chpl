// This test exercises using the value of the param field as part of the default
// initialization for a concrete field.  The concrete field is explicitly set
// in the initializer.

record Foo {
  param p: int;
  var x = p * 3;

  proc init(param p) {
    this.p = p;
    x = p + 2;
  }
}

var foo1: Foo(2);

writeln(foo1);
