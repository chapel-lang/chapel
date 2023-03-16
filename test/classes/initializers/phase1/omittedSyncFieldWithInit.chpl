class Foo {
  var s: sync int = 4;
  var other: int;

  proc init(otherVal) {
    other = otherVal;
  }
}

var ownFoo1 = new owned Foo(3);
var foo1 = ownFoo1.borrow();
writeln(foo1.s.readFE()); // the omitted initialization should set s to full
