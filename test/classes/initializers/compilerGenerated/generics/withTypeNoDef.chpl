class Foo {
  type t;
  var x: t;
}

var foo = (new owned Foo(int)).borrow();

writeln(foo.type: string);
writeln(foo);
