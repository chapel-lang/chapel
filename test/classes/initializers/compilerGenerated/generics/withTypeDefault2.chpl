class Foo {
  type t = bool;
  var x: t;
  var y = 12*3; // Verifies I haven't accidentally aborted resolution
}

var foo = new Foo(int);

writeln(foo.type: string);
writeln(foo);

var foo2 = new Foo();
writeln(foo2.type: string);
writeln(foo2);

delete foo;
delete foo2;
