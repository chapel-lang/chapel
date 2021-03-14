// This test exercises having multiple type fields and only using a different
// value for the second one.

record Foo {
  type t1;
  var x1: t1;
  type t2;
  var x2: t2;

  proc init(type t1Val, type t2Val) {
    t1 = t1Val;
    t2 = t2Val;
  }
}

var foo1 = new Foo(int, int);
var foo2 = new Foo(int, bool);

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);
