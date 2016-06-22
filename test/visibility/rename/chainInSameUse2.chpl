module Foo {
  var a = -12;

  var b = "fie";

  var bleep = false;
}

module M {
  use Foo only a as bleep, bleep as bloop;
  // In this test, for some reason the user wants to use bleep to refer to
  // a symbol that isn't Foo.bleep, but they still want to access Foo.bleep.
  // We'll warn them about the multiple mentions of the same name (and
  // hopefully they'll come to their senses), but otherwise leave it.

  proc main() {
    writeln(bloop);
    writeln(bleep);
  }
}
