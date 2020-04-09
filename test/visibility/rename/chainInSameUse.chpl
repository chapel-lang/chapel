module Foo {
  var a = -12;

  var b = "fie";
}

module M {
  use Foo only a as bleep, bleep as bloop;
  // In this test, the user has attempted to create a rename chain within a
  // single use.  Compile as if the use of "bleep" as an old name was intended
  // to refer to a bleep within Foo itself, and warn the user in case they
  // really wanted to rename a to bloop.

  proc main() {
    writeln(bloop);
  }
}
