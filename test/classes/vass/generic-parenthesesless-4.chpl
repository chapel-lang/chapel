class C {
  param x;
}

proc C.f2 {
  return 22;
}

proc C.g2: int {
  return f2;
}

var c = new shared C(1);

writeln(c.g2);
