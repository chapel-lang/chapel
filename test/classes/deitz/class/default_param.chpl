class C {
  param p: int = 1;
}

var c: C;

c = new unmanaged C();

writeln("{p = ", c.p, "}");

delete c;
