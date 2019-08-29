// Mod of test/types/type_variables/bradc/test_point_of_instantiation2-blc.chpl
// to access methods defined in using module.

// Verifies that private doesn't break this case either.
module M1 {
  private use M2;
  proc foo() {
    var c: C = new C(3);
    writeln("foo");
    bar(c); // Works
  }
  proc C.goo() {
    writeln("M1's C.goo");
  }

  proc main() {
    var c: C = new C(3);
    foo();
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

  // If baz is generic, this call should be okay because baz will get
  // instantiated in M1.
  proc baz(c1) {
    var c2: C = new C(3);

    bar(c1); // Works when called from M1
    bar(c2); // Works when called from M1
  }
}
