// Modified from
// test/classes/dinan/user_def_unique_constructor_const.chpl
class C {
  const x: int;

  proc init(y, z) {
    x = y+z;
    super.init();
  }
}

var c = new C(2, 3);

writeln(c.x);

delete c;
