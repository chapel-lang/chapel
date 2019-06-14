// Copy of test/types/type_variables/deitz/test_point_of_instantiation.chpl
// Verifies that private uses don't break point of instantiation
module M1 {
  record R { var x: int; }
  proc R.foo() { writeln("R.foo"); }

  proc g(x: R) {
    x.foo();
  }
}

module M2 {
  proc callG(x) {
    g(x);
  }
}

module M3 {
  private use M1, M2;
  // Even though g is visible to us via a private use, M2's callG should still
  // be able to apply to it.
  proc main() {
    var r: R;
    callG(r);
  }
}
