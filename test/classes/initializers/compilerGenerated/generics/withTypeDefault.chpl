class Foo {
  type t = bool;
  var x: t;
}

var foo = (new owned Foo(int)).borrow();

writeln(foo.type: string);
writeln(foo);

var foo2 = (new owned Foo()).borrow();
writeln(foo2.type: string);
writeln(foo2);
