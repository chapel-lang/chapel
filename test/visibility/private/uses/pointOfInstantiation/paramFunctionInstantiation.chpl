// Mod of test/types/type_variables/bradc/test_point_of_instantiation2-blc.chpl
// Verifies that private uses don't affect an instantiated function's ability
// to resolve its symbols
module M1 {
  private use M2;
  proc foo() {
    writeln("foo");
    bar(1); // Accesses M2's goo.
  }
  proc goo() {
    writeln("M1's goo");
  }
  proc main() {
    foo();
    baz();
    bar(2); // Accesses M2's goo.
  }
}

module M2 {
  proc goo() {
    writeln("M2's goo");
  }
  proc bar(param p: int) {
    writeln("bar");
    goo();
  }
  proc baz() {
    bar(1); // I think this should access M2's goo.
    bar(2);
    goo();
  }
}
