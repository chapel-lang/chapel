// Modified from
// test/classes/dinan/user_def_default_constructor.chpl
class C {
  var x: int;

  proc init() {
    x = 5;
  }
}

var c = new C();

writeln(c.x);

delete c;
