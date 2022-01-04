module Main {
  module Foo {
    deprecated "x is deprecated" var x: int;
    var y: bool;
  }

  proc main() {
    import this.Foo.{x as z}; // Should trigger warning

    writeln(z); // Should also trigger warning
  }
}
