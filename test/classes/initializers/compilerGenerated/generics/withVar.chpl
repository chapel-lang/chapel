class Foo {
  var v;
}

var foo = (new owned Foo(7)).borrow();
writeln(foo.type: string);
writeln(foo);
