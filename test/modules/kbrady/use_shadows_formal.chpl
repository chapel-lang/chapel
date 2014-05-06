module Foo {
  var bar = "bad";
}

proc test(addr: int) {
  use Foo;
  writeln(addr);
}

test(1);
