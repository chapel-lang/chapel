class C {
  param x;
}

proc C.f2: int {
  return 22;
}

proc C.g2 {
  return f2;
}

var c = new unmanaged C(1);

writeln(c.g2);

delete c;

