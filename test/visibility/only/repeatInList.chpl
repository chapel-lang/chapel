module Foo {
  var a = 14.2;

  const b: bool;

  proc c() {
    writeln("wheeeee");
  }

  proc d(x: int) {
    return x * 2 + 4;
  }
}

module M {
  use Foo only a, a;
  // Verifies that the user is informed when they repeat an identifier in an
  // only list.

  proc main() {
    writeln(a);
  }
}
