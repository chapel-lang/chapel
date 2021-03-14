// This test exercises using the value of the param field as part of the default
// initialization for a concrete field.  The concrete field is explicitly set
// in the initializer.

record Foo {
  param p: int;
  var x = p * 3;

  proc init(param pVal) {
    p = pVal;
    x = p + 2;
  }
}

var foo1 = new Foo(2);

writeln(foo1);
