// Exercises just the param case for classes via new, so I can verify that
// works.

class Foo {
  param a: bool;

  proc init(param a) {
    this.a = a;
  }
}

var ownF1 = new owned Foo(true);
var f1 = ownF1.borrow();
var ownF2 = new owned Foo(false);
var f2 = ownF2.borrow();
writeln(f1.type:string);
writeln(f2.type:string);
