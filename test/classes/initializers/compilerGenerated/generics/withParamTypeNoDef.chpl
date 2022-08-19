class Foo {
  param p: bool;
}

var foo = (new owned Foo(true)).borrow();
writeln(foo.type: string);
