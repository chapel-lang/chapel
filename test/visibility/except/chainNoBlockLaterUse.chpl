module A {
  use B;

  var foo = 14;
  var bar = false;
}

module B {
  var foo = 95.3;
}

module M {
  use A except foo;
  use B;
  // Verifies that B's symbols are visible, even though the other except had
  // previously blocked it

  proc main() {
    writeln(bar); // Should be false;
    writeln(foo); // Should be 95.3

    innerCall();
  }

  proc innerCall() {
    // Verifies that a use of A without the except isn't affected
    use A;
    writeln(foo);
  }
}
