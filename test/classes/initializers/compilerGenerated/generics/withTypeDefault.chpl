class Foo {
  type t = bool;
  var x: t;
}

var foo = new unmanaged Foo(int);

writeln(foo.type: string);
writeln(foo);

var foo2 = new unmanaged Foo();
writeln(foo2.type: string);
writeln(foo2);

delete foo;
delete foo2;
