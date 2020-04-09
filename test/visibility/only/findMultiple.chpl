// Verifies that if 'only' is given a list, all of the symbols in it are capable
// of being found.
module A {
  proc foo() {
    writeln("whee!");
  }

  var bar = 23.6;

  var baz = false;

  type t = int(32);
}

module M {
  use A only foo, bar, t;

  proc main() {
    foo();
    writeln(bar);
    var x: t;
    writeln(x);
  }
}
