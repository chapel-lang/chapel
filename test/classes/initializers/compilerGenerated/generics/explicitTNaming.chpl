class Foo {
  type t;
}

var foo = new unmanaged Foo(t=int);
writeln(foo.type: string);
delete foo;
