// Copy of test/types/type_variables/deitz/test_point_of_instantiation.chpl
// Verifies that private uses don't break point of instantiation
module M1 {
  record R { var x: int; }
  proc R.foo() { writeln("R.foo"); }
}

module M2 {
  proc bar(x) { x.foo(); }
}

module M3 {
  private use M1;
  use M2;
  // Even though the source of our type is visible to us via a private use,
  // M2's bar should still be able to apply to it.
  proc main() {
    var r: R;
    bar(r);
  }
}
