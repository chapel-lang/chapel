class Foo {
  param p = 2;
}

var ownFoo = new owned Foo(4);
var foo = ownFoo.borrow();
writeln(foo.type: string);

var ownFoo2 = new owned Foo();
var foo2 = ownFoo2.borrow();
writeln(foo2.type: string);
