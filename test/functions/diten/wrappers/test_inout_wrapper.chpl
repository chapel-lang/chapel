class C {
  var n: real;
  def upit(inout x:real, a: real) {
    x += 1.0;
  }

  def callUpit() {
    upit(a=1, x=n);
  }
}

def main {
  var c = new C();
  c.callUpit();
  writeln(c.n);
  c.callUpit();
  writeln(c.n);
  delete c;
}
