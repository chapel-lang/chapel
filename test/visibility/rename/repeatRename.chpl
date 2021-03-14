module Foo {
  var a = -12;

  var b = "fie";
}

module M {
  use Foo only a as bleep, a as bloop;

  proc main() {
    writeln(bloop);
    writeln(bleep);
  }
}
