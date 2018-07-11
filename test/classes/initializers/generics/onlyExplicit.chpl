// Exercises just the param case for classes via new, so I can verify that
// works.

class Foo {
  param a: bool;

  proc init(param a) {
    this.a = a;
  }
}

var f1 = new Foo(true);
var f2 = new Foo(false);
writeln(f1.type:string);
writeln(f2.type:string);

delete f1;
delete f2;
