// Modified from
// test/classes/dinan/user_def_default_constructor_const.chpl
class C {
  const x: int;

  proc init() {
    x = 5;
  }
}

var c = new shared C();

writeln(c.x);
