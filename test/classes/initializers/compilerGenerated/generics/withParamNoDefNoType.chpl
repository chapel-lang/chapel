class Foo {
  param p;
}

var foo = new unmanaged Foo(4);
writeln(foo.type: string);
delete foo;
