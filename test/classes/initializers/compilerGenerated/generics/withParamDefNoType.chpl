class Foo {
  param p = 2;
}

var foo = new Foo(4);
writeln(foo.type: string);

var foo2 = new Foo();
writeln(foo2.type: string);

delete foo;
delete foo2;
