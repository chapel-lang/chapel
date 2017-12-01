proc t() type {
  return sync int;
}

class Foo {
  var s: t();
}

var foo1 = new Foo();
writeln(foo1.s.isFull);
delete foo1;
