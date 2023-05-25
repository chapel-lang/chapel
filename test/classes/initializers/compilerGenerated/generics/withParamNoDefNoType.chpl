class Foo {
  param p;
}

var foo = (new owned Foo(4)).borrow();
writeln(foo.type: string);
