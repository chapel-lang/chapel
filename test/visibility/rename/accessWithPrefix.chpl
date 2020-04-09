module Foo {
  var bar = 0.3;
}

module M {
  use Foo only bar as baz;

  proc main() {
    writeln(Foo.bar);
  }
}
