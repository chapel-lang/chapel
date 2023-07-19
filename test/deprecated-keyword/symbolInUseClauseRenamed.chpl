module Main {
  module Foo {
    @deprecated(notes="x is deprecated") var x: int;
    var y: bool;
  }

  proc main() {
    use Foo only x as z; // Should trigger warning

    writeln(z); // Should also trigger warning
  }
}
