class Foo {
  type t;
  var v: int;
}

proc genericFoo(x: unmanaged Foo(?t)) {
  writeln(t: string);
  writeln(x);
}

var foo1 = new unmanaged Foo(int, 2);
var foo2 = new unmanaged Foo(real, 4);
genericFoo(foo1);
genericFoo(foo2);
delete foo1, foo2;
