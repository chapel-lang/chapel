class Foo {
  param p = 2;
}

var foo = (new owned Foo(4)).borrow();
writeln(foo.type: string);

var foo2 = (new owned Foo()).borrow();
writeln(foo2.type: string);
