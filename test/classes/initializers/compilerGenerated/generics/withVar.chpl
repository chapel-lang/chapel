class Foo {
  var v;
}

var foo = new borrowed Foo(7);
writeln(foo.type: string);
writeln(foo);
