module Foo {
  private var a = -12;

  var b = "fie";
}

module M {
  use Foo only a as bleep;

  proc main() {
    writeln(bleep);
  }
}
