// In this test case, the module is defined first and the function second.
module M1 {
  proc foo() {
    return 3;
  }
}

proc M1() {
  return 5;
}

// We expect the function and the module names to conflict.
use M1;
writeln(foo());
writeln(M1());
