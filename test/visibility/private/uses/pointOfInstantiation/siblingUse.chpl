// Mod of test/types/type_variables/bradc/test_point_of_instantiation2-blc.chpl

// Looks at the impact of making the use of M2 (and thus potentially its
// instantiation of bar(1) and bar(2)) private.
module M1 {
  private use M2;
  use M3;
  proc foo() {
    writeln("foo");
    bar(1); // I think this should access M2's goo.
  }
  proc goo() {
    writeln("M1's goo");
  }
  proc main() {
    foo();
    baz();
    blip();
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

module M3 {
  use M2;

  proc blip () {
    // I think this should access M2's goo in both cases.
    bar(1);
    bar(2);
  }
}
