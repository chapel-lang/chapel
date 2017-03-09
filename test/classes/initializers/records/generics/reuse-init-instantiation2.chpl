// This test exercises calling the same initializer twice when a record has a
// type field.  In this version, there is an additional argument

record Foo {
  type t;
  var x: t;

  proc init(type tVal, xVal:tVal) {
    t = tVal;
    x = xVal;
    super.init();
  }
}

var foo1: Foo(int);
var foo2: Foo(int);

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);
