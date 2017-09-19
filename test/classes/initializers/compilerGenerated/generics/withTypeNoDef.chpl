class Foo {
  type t;
  var x: t;
}

var foo = new Foo(int);

writeln(foo.type: string);
writeln(foo);

delete foo;
