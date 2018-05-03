// This test exercises calling two initializers that lead to the same
// instantiation when a class has a type field

class Foo {
  type t;
  var x: t;

  proc init(type tVal) {
    t = tVal;
  }

  proc init(xVal) {
    t = xVal.type;
    x = xVal;
  }
}

var foo1 = new unmanaged Foo(int);
var foo2 = new unmanaged Foo(3);

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);

delete foo1;
delete foo2;
