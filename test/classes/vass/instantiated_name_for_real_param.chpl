class B {
  param p;
}

var b = new unmanaged B(1.0);

writeln("{p = ", b.p, "}");

delete b;
