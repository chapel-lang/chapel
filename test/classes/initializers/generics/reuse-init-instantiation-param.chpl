// This test exercises calling the same initializer twice when a class has a
// param field

class Foo {
  param p: int;
  var x: int;

  proc init(param pVal) {
    p = pVal;
  }
}

var foo1 = new borrowed Foo(2);
var foo2 = new borrowed Foo(2);

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);
