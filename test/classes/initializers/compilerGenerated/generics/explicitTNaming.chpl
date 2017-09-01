class Foo {
  type t;
}

var foo = new Foo(t=int);
writeln(foo.type: string);
delete foo;
