class Foo {
  type t;
  param p: int;
  var v;

  proc init() {
    t = int;
    v = 10;
    super.init();
  }
}

var foo = new Foo();
writeln(foo.type: string);
writeln(foo);
delete foo;
