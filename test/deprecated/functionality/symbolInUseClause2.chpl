module Foo {
  deprecated "x is deprecated" var x: int;
  var y: bool;
}

proc main() {
  use Foo except x; // Should trigger warning

  writeln(y); // Should not trigger warning
}
