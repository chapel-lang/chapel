record Foo {
  var x: int;

  proc init() {
    x = 12;
  }
}

proc outArgFoo(out val: Foo) {
  var localFoo: Foo;
  localFoo.x = 7;
  val = localFoo;
}

var foo: Foo;
writeln(foo);
var otherFoo: Foo;
outArgFoo(otherFoo);
writeln(otherFoo);
