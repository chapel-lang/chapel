module M1 {
  var foo: int = 4;
}

module M2 {
  use M1 except ?t;

  // I expect this test to fail compilation.  A query is not valid in the except
  // list.
  proc main() {
    writeln("Wheeeeee");
  }
}
