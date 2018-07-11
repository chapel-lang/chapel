class Foo {
  var arr: [1..5] int = [1, 2, 3, 4, 5];
}

var foo = new unmanaged Foo();
writeln(foo);
delete foo;
