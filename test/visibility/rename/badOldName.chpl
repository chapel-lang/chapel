module Foo {
  var bar = 42;

  var baz = 19.6;
}

module M {
  use Foo only br as boop;
  // Should fail here, br is not the name of a symbol in Foo.

  proc main() {
    writeln(boop);
  }
}
