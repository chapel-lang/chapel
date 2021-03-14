type t = int;

class Foo {
  var s: sync t;
}

var foo1 = new unmanaged Foo();
writeln(foo1.s.isFull);
delete foo1;
