// This test exercises using the value of the param field as part of the default
// initialization for a concrete field.  The concrete field is explicitly set
// in the initializer.

class Foo {
  param p: int;
  var x = p * 3;

  proc init(param pVal) {
    p = pVal;
    x = pVal + 2;
  }
}

var foo1 = new owned Foo(2);

writeln(foo1);
