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

  def testit(z:int) {
    writeln("y.x is: ", y.x, ", z is: ", z, ".");
  }
}

def main() {
  var myD = new D();
  start(myD);
  testit(myD, 7);
}
