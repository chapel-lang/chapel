class Foo {
  type t = int;
  var s: sync t;
}

var foo1 = new Foo();
writeln(foo1.s.isFull);
delete foo1;
