class Foo {
  param p: bool;
}

var foo = new unmanaged Foo();
writeln(foo.type: string);

delete foo;
