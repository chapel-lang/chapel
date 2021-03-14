//
// The tests testOpaqueClass.chpl and testOpaqueClass2.chpl are identical
// except for the writeln(c) in main is commented out in testOpaqueClass2.
// When that writeln is done, class C's symbols are made accessible.
//
module M1 {
  module M1Inner {
    var n: int;
    class C {
      var a = 2;
      proc foo() {
        return a;
      }
    }
  }
  proc retCs() {
    use M1Inner;
    return new C();
  }
}

module M2 {
  proc main {
    use M1;
    var c = retCs();
    writeln(c);
    writeln(c.a);
  }
}
