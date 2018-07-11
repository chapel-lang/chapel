class C {
  param p: int = 1;
}

var c: unmanaged C;

c = new unmanaged C();

writeln("{p = ", c.p, "}");

delete c;
