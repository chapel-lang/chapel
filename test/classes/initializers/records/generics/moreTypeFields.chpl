// This test exercises having multiple type fields and only using a different
// value for the second one.

record Foo {
  type t1;
  var x1: t1;
  type t2;
  var x2: t2;

  proc init(type t1, type t2) {
    this.t1 = t1;
    this.t2 = t2;
  }
}

var foo1: Foo(int, int);
var foo2: Foo(int, bool);

writeln(foo1.type == foo2.type);
writeln(foo1);
writeln(foo2);
