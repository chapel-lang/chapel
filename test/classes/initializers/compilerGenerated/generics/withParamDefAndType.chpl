class Foo {
  param p: int = 5;
}

var foo = (new owned Foo(7)).borrow();
writeln(foo.type: string);

var foo2 = (new owned Foo()).borrow();
writeln(foo2.type: string);
