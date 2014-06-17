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

  proc testit(z:int) {
    writeln("y.x is: ", y.x, ", z is: ", z, ".");
  }
}

proc main() {
  var myD = new D();
  start(myD);
  testit(myD, 7);
}
