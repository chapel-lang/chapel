module OuterModule {
  // In this test case, the module is defined second and the function first.
  proc M1() {
    return 5;
  }

  module M1 {
    proc foo() {
      return 3;
    }
  }

  // We expect the function and the module names to conflict.
  use M1;
  writeln(foo());
  writeln(M1());
}
