class C {
  param p: int = 1;
}

var c: C;

c = new C();

writeln("{p = ", c.p, "}");

delete c;
