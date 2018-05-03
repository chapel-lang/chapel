class Foo {
  var v;
}

var foo = new unmanaged Foo(7);
writeln(foo.type: string);
writeln(foo);

delete foo;
