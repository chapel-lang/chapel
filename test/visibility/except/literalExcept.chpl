module M1 {
  var foo: int = 4;
}

module M2 {
  use M1 except "blah blah";

  // I expect this test to fail compilation.  A string literal is not valid
  // in the except list.
  proc main() {
    writeln("Wheeeeee");
  }
}
