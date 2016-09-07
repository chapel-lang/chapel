// Modified from
// test/classes/dinan/user_def_unique_constructor.chpl
class C {
  var x: int;

  proc init(y, z) {
    x = y+z;
  }
}

var c = new C(2, 3);

writeln(c.x);

delete c;
