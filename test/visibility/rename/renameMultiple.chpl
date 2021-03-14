module Foo {
  var bar = 42;

  var baz = 19.6;
}

module M {
  use Foo only bar as boop, baz as bleep;

  proc main() {
    writeln(boop); // Should work, boop is M's name for Foo.bar
    writeln(bleep); // Should work, bleep is M's name for Foo.baz
  }
}
