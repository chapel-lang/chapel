module Foo {
  var bar = 0.3;
}

module M {
  use Foo only bar as baz;

  proc main() {
    writeln(Foo.baz);
    // Should fail, renaming bar as baz should only affect this context, not
    // Foo's context.
  }
}
