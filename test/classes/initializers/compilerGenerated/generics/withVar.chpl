class Foo {
  var v;
}

var ownFoo = new owned Foo(7);
var foo = ownFoo.borrow();
writeln(foo.type: string);
writeln(foo);
