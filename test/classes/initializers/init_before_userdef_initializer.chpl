// Modified from
// test/classes/dinan/init_before_userdef_constructor.chpl
proc f(x) {
  return x*2;
}

class C {
  var x: int = 2;
  var y: int = f(x);

  // This works around overriding the default initializer
  proc init(w: bool) {
    writeln("In user defined initializer: x=", x, " y=", y);
  }
}

var c: C = new C(true);

writeln("Got back: x=", c.x, " y=", c.y);

delete c;
