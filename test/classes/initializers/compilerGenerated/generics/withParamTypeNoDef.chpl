class Foo {
  param p: bool;
}

var ownFoo = new owned Foo(true);
var foo = ownFoo.borrow();
writeln(foo.type: string);
