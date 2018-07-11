class Foo {
  type t = sync int;
  var s: t;
}

var foo1 = new unmanaged Foo();
writeln(foo1.s.isFull);
delete foo1;
