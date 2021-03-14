module Foo {
  var bar = 42;

  var baz = 19.6;
}

module M {
  use Foo except bar as boop;
  // Should fail, don't allow renaming on except lists

  proc main() {
    writeln(boop);
  }
}
