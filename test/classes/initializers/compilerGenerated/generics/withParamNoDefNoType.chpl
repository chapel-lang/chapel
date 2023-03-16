class Foo {
  param p;
}

var ownFoo = new owned Foo(4);
var foo = ownFoo.borrow();
writeln(foo.type: string);
