class C {
  var x: int;

  proc init() {
    x = 5;
  }
}

var c = new unmanaged C();

writeln(c.x);

delete c;
