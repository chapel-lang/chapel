class C {
  const x: int;

  proc C(y, z) {
    x = y+z;
  }
}

var c = new C(2, 3);

writeln(c.x);

delete c;
