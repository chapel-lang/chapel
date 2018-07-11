// Modified from
// test/classes/dinan/user_def_unique_constructor_const_multipleassign.chpl
class C {
  const x: int;

  proc init(y, z) {
    x = y + z;
    x = 1;

  }
}

var c = new C(2, 3);

writeln(c.x);

delete c;
