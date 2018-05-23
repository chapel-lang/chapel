class Foo {
  var s: sync int = 4;
  var other: int;

  proc init(otherVal) {
    other = otherVal;
  }
}

var foo1 = new borrowed Foo(3);
writeln(foo1.s.readFE()); // the omitted initialization should set s to full
