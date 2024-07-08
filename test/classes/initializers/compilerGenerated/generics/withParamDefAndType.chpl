class Foo {
  param p: int = 5;
}

var ownFoo = new owned Foo(7);
var foo = ownFoo.borrow();
writeln(foo.type: string);

var ownFoo2 = new owned Foo();
var foo2 = ownFoo2.borrow();
writeln(foo2.type: string);
