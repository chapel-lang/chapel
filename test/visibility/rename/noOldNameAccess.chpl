module Foo {
  var bar = 42;
}

module M {
  use Foo only bar as baz;

  proc main() {
    writeln(bar); // Should fail, bar should not be visible as a name.
  }
}
