// This test exercises using the value of the param field as part of the default
// initialization for a concrete field.  The concrete field is omitted in the
// initializer.

record Foo {
  param p: int;
  var x = p * 3;

  proc init(param pVal) {
    p = pVal;
  }
}

var foo1: Foo(2) = new Foo(2);

writeln(foo1);
