class Foo {
  param p: int = 5;
}

var foo = new borrowed Foo(7);
writeln(foo.type: string);

var foo2 = new borrowed Foo();
writeln(foo2.type: string);
