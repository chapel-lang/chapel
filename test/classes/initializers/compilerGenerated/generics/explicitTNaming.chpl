class Foo {
  type t;
}

var foo = (new owned Foo(t=int)).borrow();
writeln(foo.type: string);
