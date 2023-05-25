class Foo {
  type t = bool;
  var x: t;
  var y = 12*3; // Verifies I haven't accidentally aborted resolution
}

var foo = (new owned Foo(int)).borrow();

writeln(foo.type: string);
writeln(foo);

var foo2 = (new owned Foo()).borrow();
writeln(foo2.type: string);
writeln(foo2);
