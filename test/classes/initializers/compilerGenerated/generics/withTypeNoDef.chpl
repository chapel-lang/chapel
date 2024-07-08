class Foo {
  type t;
  var x: t;
}

var ownFoo = new owned Foo(int);
var foo = ownFoo.borrow();

writeln(foo.type: string);
writeln(foo);
