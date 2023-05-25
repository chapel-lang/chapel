module Main {
  module Foo {
    @deprecated(notes="x is deprecated") var x: int;
    var y: bool;
  }

  proc main() {
    use Foo except x; // Should trigger warning

    writeln(y); // Should not trigger warning
  }
}
