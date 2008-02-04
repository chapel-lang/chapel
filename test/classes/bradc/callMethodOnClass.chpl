class C {
  var x = 1.2;
}

def newC() {
  var c = new C();
  c.x = 2.3;
  return c;
}

class D {
  var y: C;

  def start {
    y = newC();
  }

  def testit {
    writeln("y.x is: ", y.x);
  }
}

def main() {
  var myD = new D();
  D.start;  // BUG if I call this on D!!
  D.testit;
}
