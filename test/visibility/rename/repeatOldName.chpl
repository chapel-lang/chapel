module Foo {
  var a = -12;

  var b = "fie";
}

module M {
  use Foo only a as bleep, a;

  proc main() {
    writeln(a);
    writeln(bleep);
  }
}
