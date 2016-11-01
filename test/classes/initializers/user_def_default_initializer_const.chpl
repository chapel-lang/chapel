// Modified from
// test/classes/dinan/user_def_default_constructor_const.chpl
class C {
  const x: int;

  proc init() {
    x = 5;
    super.init();
  }
}

var c = new C();

writeln(c.x);

delete c;
