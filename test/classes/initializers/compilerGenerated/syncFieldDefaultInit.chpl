class Foo {
  var x$: sync int = 5;
}

var foo = new unmanaged Foo();
writeln(foo.x$.readFF());
delete foo;
