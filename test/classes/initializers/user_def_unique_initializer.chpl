class C {
  var x: int;

  proc init(y, z) {
    x = y+z;
  }
}

var c = new unmanaged C(2, 3);

writeln(c.x);

delete c;
