class C {
  const x: int;

  proc C() {
    x = 5;
  }
}

var c = new unmanaged C();

writeln(c.x);

delete c;
