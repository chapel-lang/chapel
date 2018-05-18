class Foo {
  type t;
  var v: int;
}

proc takesUnmanagedFoo(x: unmanaged Foo) {
  writeln(x);
}

var foo1 = new unmanaged Foo(int, 3);
var foo2 = new unmanaged Foo(real, 4);
takesUnmanagedFoo(foo1);
takesUnmanagedFoo(foo2);
delete foo1, foo2;
