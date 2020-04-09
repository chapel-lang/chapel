// Mod of test/types/type_variables/bradc/test_point_of_instantiation2-blc.chpl
// to access methods defined in using module.

// Verifies that private doesn't break this case either.
module M1 {
  private use M2;
  proc C.goo() {
    writeln("M1's C.goo");
  }

  proc main() {
    var c: borrowed C = new C(3);
    baz(c);
  }
}

module M2 {
  class C {
    var a: int;

    proc someMethod() {
      writeln("In C.someMethod()");
    }
  }

  proc bar(c) {
    writeln("bar");
    c.goo();
  }

  // if baz is not generic and is called from M2.main (and there is nothing to
  // instantiate bar before it), this should error.
  proc baz(c1: borrowed C) {
    var c2: borrowed C = new C(3);

    bar(c1);
    bar(c2);
  }
}
