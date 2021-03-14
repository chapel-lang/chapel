class Foo {
  type t;
  var x: t;
}

var foo = new borrowed Foo(int);

writeln(foo.type: string);
writeln(foo);
