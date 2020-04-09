module Foo {
  var bar = 42;

  var baz = 19.6;
}

module M {
  use Foo only bar as baz, baz;
  // Should cause a naming conflict on the use statement.  Arbitrarily choosing
  // one of them would be confusing.

  proc main() {
    writeln(baz);
  }
}
