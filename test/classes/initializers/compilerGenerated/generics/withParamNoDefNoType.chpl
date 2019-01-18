class Foo {
  param p;
}

var foo = new borrowed Foo(4);
writeln(foo.type: string);
