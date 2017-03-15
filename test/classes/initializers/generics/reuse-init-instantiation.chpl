// This test exercises calling the same initializer twice when a class has a
// type field

class Foo {
  type t;
  var x: t;

  proc init(type tVal) {
    t = tVal;
    super.init();
  }
}

var foo1 = new Foo(int);
var foo2 = new Foo(int);

writeln(foo1.type == foo2.type);

delete foo1;
delete foo2;
