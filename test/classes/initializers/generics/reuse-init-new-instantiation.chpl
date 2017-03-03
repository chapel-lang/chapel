// This test exercises calling the same initializer twice when a class has a
// type field, but using different values for the generic argument

class Foo {
  type t;
  var x: t;

  proc init(type tVal) {
    t = tVal;
    super.init();
  }
}

var foo1 = new Foo(int);
var foo2 = new Foo(bool);

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);

delete foo1;
delete foo2;
