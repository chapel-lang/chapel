class C {
  var x = 1.2;
}

fun newC() {
  var c = C();
  c.x = 2.3;
  return c;
}

class D {
  var y: C;

  fun start {
    y = newC();
  }

  fun testit {
    writeln("y.x is: ", y.x);
  }
}

fun main() {
  var myD = D();
  D.start;  -- BUG if I call this on D!!
  D.testit;
}
