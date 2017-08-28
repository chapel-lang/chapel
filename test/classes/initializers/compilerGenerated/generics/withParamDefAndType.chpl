class Foo {
  param p: int = 5;
}

var foo = new Foo(7);
writeln(foo.type: string);

var foo2 = new Foo();
writeln(foo2.type: string);

delete foo;
delete foo2;
