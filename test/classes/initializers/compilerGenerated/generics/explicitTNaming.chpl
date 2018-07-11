class Foo {
  type t;
}

var foo = new borrowed Foo(t=int);
writeln(foo.type: string);
