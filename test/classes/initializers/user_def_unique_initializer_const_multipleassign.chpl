// Modified from
// test/classes/dinan/user_def_unique_initializer_const_multipleassign.chpl
class C {
  const x: int;

  proc init(y, z) {
    // Phase 1
    x = y+z;
    this.complete();
    // Phase 2
    x -= 1; // Should fail, can't modify const fields in Phase 2
  }
}

var c = new C(2, 3);

writeln(c.x);

delete c;
