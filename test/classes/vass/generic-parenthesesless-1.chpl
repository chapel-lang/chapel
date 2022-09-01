class C {
  param x: int;
}

proc C.f2 {
  return 22;
}

proc C.g2 {
  return f2;
}

var c = (new owned C(1)).borrow();

writeln(c.g2);
