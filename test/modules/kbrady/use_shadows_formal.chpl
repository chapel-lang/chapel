module Foo {
  var bar = "bad";
}

proc test(bar: int) {
  use Foo;
  writeln(bar);
}

test(1);
