class C {
  var n: real;
  proc upit(inout x:real, a: real) {
    x += 1.0;
  }

  proc callUpit() {
    upit(a=1, x=n);
  }
}

proc main {
  var c = new C();
  c.callUpit();
  writeln(c.n);
  c.callUpit();
  writeln(c.n);
  delete c;
}
