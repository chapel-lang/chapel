// Exercises just the param case for classes via new, so I can verify that
// works.

class Foo {
  param a: bool;

  proc init(param a) {
    this.a = a;
  }
}

var f1 = (new owned Foo(true)).borrow();
var f2 = (new owned Foo(false)).borrow();
writeln(f1.type:string);
writeln(f2.type:string);
