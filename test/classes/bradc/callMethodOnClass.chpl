class C {
  var x = 1.2;
}

proc newC() {
  var c = new C();
  c.x = 2.3;
  return c;
}

class D {
  var y: C;

  proc start {
    y = newC();
  }

  proc testit {
    writeln("y.x is: ", y.x);
  }
}

proc main() {
  var myD = new D();
  D.start;  // BUG if I call this on D!!
  D.testit;
}
