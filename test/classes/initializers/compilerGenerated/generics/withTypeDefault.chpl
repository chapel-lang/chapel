class Foo {
  type t = bool;
  var x: t;
}

var ownFoo = new owned Foo(int);
var foo = ownFoo.borrow();

writeln(foo.type: string);
writeln(foo);

var ownFoo2 = new owned Foo();
var foo2 = ownFoo2.borrow();
writeln(foo2.type: string);
writeln(foo2);
