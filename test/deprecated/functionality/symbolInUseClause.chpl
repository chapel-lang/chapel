module Foo {
  deprecated "x is deprecated" var x: int;
  var y: bool;
}

proc main() {
  use Foo only x; // Should trigger warning

  writeln(x); // Should also trigger warning
}
