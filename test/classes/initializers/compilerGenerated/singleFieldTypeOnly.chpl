class Foo {
  var s: single int;
}

var foo1 = new unmanaged Foo();
writeln(foo1.s.isFull);
delete foo1;
