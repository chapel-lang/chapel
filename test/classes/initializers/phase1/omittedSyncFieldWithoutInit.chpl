class Foo {
  var s: sync int;
  var other: int;

  proc init(otherVal) {
    other = otherVal;
  }
}

var foo1 = new unmanaged Foo(3);
writeln(foo1.s.isFull); // the omitted initialization should leave s empty
delete foo1;
