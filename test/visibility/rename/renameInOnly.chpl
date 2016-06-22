module Foo {
  var bar = 42;

  var baz = 19.6;
}

module M {
  use Foo only bar as boop;

  proc main() {
    writeln(boop); // Should work, boop is M's name for Foo.bar
  }
}
