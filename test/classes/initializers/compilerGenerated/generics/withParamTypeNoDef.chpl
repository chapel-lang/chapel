class Foo {
  param p: bool;
}

var foo = new Foo(true);
writeln(foo.type: string);

delete foo;
