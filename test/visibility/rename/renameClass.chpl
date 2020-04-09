module Foo {
  class bar {
    var a = 15;

    proc baz () {
      writeln("In Foo's bar.baz() method, with a = ", a);
    }
  }
}

module M {
  use Foo only bar as bleep;

  proc main() {
    var instance = new borrowed bleep();

    instance.baz();
  }
}
