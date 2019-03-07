class Foo {
  type t;
  param p: int;
  var v;

  proc init() {
    t = int;
    v = 10;
  }
}

var foo = new unmanaged Foo();
writeln(foo.type: string);
writeln(foo);
delete foo;
