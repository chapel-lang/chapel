class Foo {
  param p: bool;
}

var foo = new borrowed Foo(true);
writeln(foo.type: string);
