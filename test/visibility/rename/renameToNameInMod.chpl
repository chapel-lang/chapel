module Foo {
  var bar = 42;

  var baz = 19.6;
}

module M {
  use Foo only bar as baz;

  proc main() {
    writeln(baz);
    // Should work, but use Foo.bar.  Foo.baz is not visible as just baz.
  }
}
