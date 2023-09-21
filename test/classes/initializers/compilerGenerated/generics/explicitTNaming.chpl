class Foo {
  type t;
}

var ownFoo = new owned Foo(t=int);
var foo = ownFoo.borrow();
writeln(foo.type: string);
