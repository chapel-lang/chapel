class Foo {
  param p = 2;
}

var foo = new borrowed Foo(4);
writeln(foo.type: string);

var foo2 = new borrowed Foo();
writeln(foo2.type: string);
