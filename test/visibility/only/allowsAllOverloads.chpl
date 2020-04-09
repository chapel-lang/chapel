// Verifies that 'only' identifiers are applied to all overloads of a function
// instead of just some arbitrarily chosen one.
module allTheFoos {
  proc foo() {
    writeln("Rogue 2, standing by");
  }

  proc foo(x: int) {
    writeln("Red ", x, ", standing by");
  }

  proc foo(a = false, b: bool) {
    if (a || b) then
      writeln("All present and accounted for");
    else
      writeln("Well, I guess that's something at any rate");
  }

  proc ignored() {
    writeln("I was feeling left out, so I thought I'd drop by");
  }
}

module M {
  use allTheFoos only foo;

  proc main() {
    foo();
    foo(3);
    foo(true, false);
  }
}
