class C {
  param x: int;
}

proc C.f2 {
  return 22;
}

proc C.g2 {
  return f2;
}

var c = new C(1);

writeln(c.g2);

delete c;

