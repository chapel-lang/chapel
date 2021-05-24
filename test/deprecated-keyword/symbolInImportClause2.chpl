module Main {
  module Foo {
    deprecated "x is deprecated" var x: int;
    var y: bool;
  }

  proc main() {
    import this.Foo.{x}; // Should trigger warning

    writeln(x); // Should also trigger warning
  }
}
