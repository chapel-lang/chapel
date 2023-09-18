class C {
  param x: int;
}

proc C.f2 {
  return 22;
}

proc C.g2 {
  return f2;
}

var ownC = new owned C(1);
var c = ownC.borrow();

writeln(c.g2);
